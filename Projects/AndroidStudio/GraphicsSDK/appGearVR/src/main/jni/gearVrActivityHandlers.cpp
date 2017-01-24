/**
 * Native Activity lifecyle callbacks
 */

#include <jni.h>
#include <android/input.h>


// Core Engine modules
#include <coreEngine/components/transform/TransformCameraFactory.h>
#include <coreEngine/components/transform/TransformModelFactory.h>
#include <coreEngine/events/EventQueue.h>

// GLImplementation Modules
#include <glImplementation/factory/SceneGLFactory.h>
#include <glImplementation/factory/ModelGLFactory.h>
#include <glImplementation/factory/opengles3/DiffuseTextureGLES3Factory.h>
#include <glImplementation/factory/opengles3/DiffuseTextureCubeMapGLES3Factory.h>

// Image360 Application
#include <image360/Image360.h>

// Android Modules
#include <LoggerAndroidFactory.h>
#include <MutexLockAndroid.h>
#include <CameraGLOVRFactory.h>
#include <ImageBMPLoaderAndroid.h>
#include <RendererGearVR.h>

#ifdef __cplusplus
extern "C" {
#endif


using namespace cl;

/*
 * TODO: Get rid of these ugly global variables
 *          move code to CPP 11 standard - use classes instead of
 *          structs etc. etc.
 */

// Logger, Event Queue and Application
std::unique_ptr<LoggerFactoryAndroid> loggerFactory = nullptr;
std::unique_ptr<ILogger> logger = nullptr;
std::unique_ptr<IEventQueue> eventQueue = nullptr;


/*
====================================================================================================

                                        ovrMessageQueue

====================================================================================================
*/

typedef enum {
    MQ_WAIT_NONE,
    MQ_WAIT_RECEIVED,
    MQ_WAIT_PROCESSED
} ovrMQWait;


#define MAX_MESSAGE_PARMS    8
#define MAX_MESSAGES        1024

typedef struct {
    int Id;
    ovrMQWait Wait;
    long long Parms[MAX_MESSAGE_PARMS];
} ovrMessage;

static void ovrMessage_Init(ovrMessage *message, const int id, const ovrMQWait wait) {
    message->Id = id;
    message->Wait = wait;
    memset(message->Parms, 0, sizeof(message->Parms));
}

static void ovrMessage_SetPointerParm(ovrMessage *message, int index,
                                      void *ptr) { *(void **) &message->Parms[index] = ptr; }
static void *ovrMessage_GetPointerParm(ovrMessage *message,
                                       int index) { return *(void **) &message->Parms[index]; }
static void ovrMessage_SetIntegerParm(ovrMessage *message, int index,
                                      int value) { message->Parms[index] = value; }
static int ovrMessage_GetIntegerParm(ovrMessage *message,
                                     int index) { return (int) message->Parms[index]; }
static void ovrMessage_SetFloatParm(ovrMessage *message, int index,
                                    float value) { *(float *) &message->Parms[index] = value; }
static float ovrMessage_GetFloatParm(ovrMessage *message,
                                     int index) { return *(float *) &message->Parms[index]; }


// Cyclic queue with messages.
typedef struct {
    ovrMessage Messages[MAX_MESSAGES];
    volatile int Head;
    volatile int Tail;
    ovrMQWait Wait;

    volatile bool EnabledFlag;
    volatile bool PostedFlag;
    volatile bool ReceivedFlag;
    volatile bool ProcessedFlag;

    pthread_mutex_t Mutex;
    pthread_cond_t PostedCondition;
    pthread_cond_t ReceivedCondition;
    pthread_cond_t ProcessedCondition;
} ovrMessageQueue;

static void ovrMessageQueue_Create(ovrMessageQueue *messageQueue) {
    messageQueue->Head = 0;
    messageQueue->Tail = 0;
    messageQueue->Wait = MQ_WAIT_NONE;
    messageQueue->EnabledFlag = false;
    messageQueue->PostedFlag = false;
    messageQueue->ReceivedFlag = false;
    messageQueue->ProcessedFlag = false;

    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
    pthread_mutex_init(&messageQueue->Mutex, &attr);
    pthread_cond_init(&messageQueue->PostedCondition, NULL);
    pthread_cond_init(&messageQueue->ReceivedCondition, NULL);
    pthread_cond_init(&messageQueue->ProcessedCondition, NULL);
}

static void ovrMessageQueue_Destroy(ovrMessageQueue *messageQueue) {
    pthread_mutex_destroy(&messageQueue->Mutex);
    pthread_cond_destroy(&messageQueue->PostedCondition);
    pthread_cond_destroy(&messageQueue->ReceivedCondition);
    pthread_cond_destroy(&messageQueue->ProcessedCondition);
}

static void ovrMessageQueue_Enable(ovrMessageQueue *messageQueue, const bool set) {
    messageQueue->EnabledFlag = set;
}

static void ovrMessageQueue_PostMessage(ovrMessageQueue *messageQueue, const ovrMessage *message) {
    if (!messageQueue->EnabledFlag) {
        return;
    }
    while (messageQueue->Tail - messageQueue->Head >= MAX_MESSAGES) {
        usleep(1000);
    }
    pthread_mutex_lock(&messageQueue->Mutex);
    messageQueue->Messages[messageQueue->Tail & (MAX_MESSAGES - 1)] = *message;
    messageQueue->Tail++;
    messageQueue->PostedFlag = true;
    pthread_cond_broadcast(&messageQueue->PostedCondition);
    if (message->Wait == MQ_WAIT_RECEIVED) {
        while (!messageQueue->ReceivedFlag) {
            pthread_cond_wait(&messageQueue->ReceivedCondition,
                              &messageQueue->Mutex);
        }
        messageQueue->ReceivedFlag = false;
    }
    else if (message->Wait == MQ_WAIT_PROCESSED) {
        while (!messageQueue->ProcessedFlag) {
            pthread_cond_wait(&messageQueue->ProcessedCondition, &messageQueue->Mutex);
        }
        messageQueue->ProcessedFlag = false;
    }
    pthread_mutex_unlock(&messageQueue->Mutex);
}

static void ovrMessageQueue_SleepUntilMessage(ovrMessageQueue *messageQueue) {
    if (messageQueue->Wait == MQ_WAIT_PROCESSED) {
        messageQueue->ProcessedFlag = true;
        pthread_cond_broadcast(&messageQueue->ProcessedCondition);
        messageQueue->Wait = MQ_WAIT_NONE;
    }
    pthread_mutex_lock(&messageQueue->Mutex);
    if (messageQueue->Tail > messageQueue->Head) {
        pthread_mutex_unlock(&messageQueue->Mutex);
        return;
    }
    while (!messageQueue->PostedFlag) {
        pthread_cond_wait(&messageQueue->PostedCondition, &messageQueue->Mutex);
    }
    messageQueue->PostedFlag = false;
    pthread_mutex_unlock(&messageQueue->Mutex);

}

static bool ovrMessageQueue_GetNextMessage(ovrMessageQueue *messageQueue, ovrMessage *message,
                                           bool waitForMessages) {
    if (messageQueue->Wait == MQ_WAIT_PROCESSED) {
        messageQueue->ProcessedFlag = true;
        pthread_cond_broadcast(&messageQueue->ProcessedCondition);
        messageQueue->Wait = MQ_WAIT_NONE;
    }
    if (waitForMessages) {
        ovrMessageQueue_SleepUntilMessage(messageQueue);
    }
    pthread_mutex_lock(&messageQueue->Mutex);
    if (messageQueue->Tail <= messageQueue->Head) {
        pthread_mutex_unlock(&messageQueue->Mutex);
        return false;
    }
    *message = messageQueue->Messages[messageQueue->Head & (MAX_MESSAGES - 1)];
    messageQueue->Head++;
    pthread_mutex_unlock(&messageQueue->Mutex);
    if (message->Wait == MQ_WAIT_RECEIVED) {
        messageQueue->ReceivedFlag = true;
        pthread_cond_broadcast(&messageQueue->ReceivedCondition);
    }
    else if (message->Wait == MQ_WAIT_PROCESSED) {
        messageQueue->Wait = MQ_WAIT_PROCESSED;
    }
    return true;

}


/*
====================================================================================================

                                        ovrAppThread

====================================================================================================
*/

enum {
    MESSAGE_ON_CREATE,
    MESSAGE_ON_START,
    MESSAGE_ON_RESUME,
    MESSAGE_ON_PAUSE,
    MESSAGE_ON_STOP,
    MESSAGE_ON_DESTROY,
    MESSAGE_ON_SURFACE_CREATED,
    MESSAGE_ON_SURFACE_DESTROYED,
    MESSAGE_ON_KEY_EVENT,
    MESSAGE_ON_TOUCH_EVENT
};

typedef struct {
    Image360 *Application;
    AAssetManager *mgr;
//    ovrMobile **ovrM;
    jobject Activity;
    const char *appDir;
    bool Resumed;
    bool Started;
    pthread_t Thread;
    ovrMessageQueue MessageQueue;
    ANativeWindow *NativeWindow;
} ovrAppThread;


void *AppThreadFunction(void *parm) {
    ovrAppThread *appThread = (ovrAppThread *) parm;

    auto renderer = (RendererGearVR *) appThread->Application->getRenderer();

    appThread->Application->start();

    for (bool destroyed = false; destroyed == false;) {
        for (; ;) {
            ovrMessage message;
            const bool waitForMessages = (renderer->getOvr() == NULL && destroyed == false);
            if (!ovrMessageQueue_GetNextMessage(&appThread->MessageQueue, &message,
                                                waitForMessages)) {
                break;
            }

            switch (message.Id) {
                case MESSAGE_ON_CREATE: {
                    break;
                }
                case MESSAGE_ON_START: {
                    break;
                }
                case MESSAGE_ON_RESUME: {
                    appThread->Resumed = true;
                    break;
                }
                case MESSAGE_ON_PAUSE: {
                    appThread->Resumed = false;
                    break;
                }
                case MESSAGE_ON_STOP: {
                    break;
                }
                case MESSAGE_ON_DESTROY: {
                    renderer->setWindow(NULL);
                    destroyed = true;
                    break;
                }
                case MESSAGE_ON_SURFACE_CREATED: {
                    auto newNativeWindow = (ANativeWindow *) ovrMessage_GetPointerParm(&message, 0);
                    renderer->setWindow(newNativeWindow);
                    break;
                }
                case MESSAGE_ON_SURFACE_DESTROYED: {
                    renderer->setWindow(NULL);
                    break;
                }
                case MESSAGE_ON_KEY_EVENT: {
                    break;
                }
                case MESSAGE_ON_TOUCH_EVENT: {
                    break;
                }

            }
            renderer->handleVrModeChanges(appThread->Resumed);
        }

        if (renderer->getOvr() == NULL) {
            continue;
        }

        if (!appThread->Started) {
            ImageBMPLoaderAndroid imageBMPLoader(logger.get());
            std::vector<std::unique_ptr<Image> > textureImages;
            TEXTURE_MAP_MODE mode = CUBE_MAP_MODE_SINGLE_IMAGE;

            switch (mode) {
                case CUBE_MAP_MODE_SINGLE_IMAGE:
                    textureImages.push_back(
                            imageBMPLoader.loadImage(std::string(appThread->appDir)
                                                     + std::string("/chymeraSDKAssets/image360/image360Ad.bmp")));
                    break;
                case CUBE_MAP_MODE_SIX_IMAGES:

                    textureImages.push_back(imageBMPLoader.loadImage(std::string(appThread->appDir)
                                                                     + std::string("/chymeraSDKAssets/image360/cubemap_geo_front.bmp")));
                    textureImages.push_back(imageBMPLoader.loadImage(std::string(appThread->appDir)
                                                                     + std::string("/chymeraSDKAssets/image360/cubemap_geo_left.bmp")));
                    textureImages.push_back(imageBMPLoader.loadImage(std::string(appThread->appDir)
                                                                     + std::string("/chymeraSDKAssets/image360/cubemap_geo_back.bmp")));
                    textureImages.push_back(imageBMPLoader.loadImage(std::string(appThread->appDir)
                                                                     + std::string("/chymeraSDKAssets/image360/cubemap_geo_right.bmp")));
                    textureImages.push_back(imageBMPLoader.loadImage(std::string(appThread->appDir)
                                                                     + std::string("/chymeraSDKAssets/image360/cubemap_geo_top.bmp")));
                    textureImages.push_back(imageBMPLoader.loadImage(std::string(appThread->appDir)
                                                                     + std::string("/chymeraSDKAssets/image360/cubemap_geo_bottom.bmp")));
                    break;
                case EQUIRECTANGULAR_MAP_MODE:
                    textureImages.push_back(
                            imageBMPLoader.loadImage(appThread->mgr, "images/tex_current.bmp"));
                    break;
            }


//            switch (mode) {
//                case CUBE_MAP_MODE_SINGLE_IMAGE:
//                    textureImages.push_back(
//                            imageBMPLoader.loadImage(appThread->mgr, "images/cubemap_current.bmp"));
//                    break;
//                case CUBE_MAP_MODE_SIX_IMAGES:
//                    textureImages.push_back(imageBMPLoader.loadImage(appThread->mgr,
//                                                                     "images/cubemap_geo_front.bmp"));
//                    textureImages.push_back(imageBMPLoader.loadImage(appThread->mgr,
//                                                                     "images/cubemap_geo_left.bmp"));
//                    textureImages.push_back(imageBMPLoader.loadImage(appThread->mgr,
//                                                                     "images/cubemap_geo_back.bmp"));
//                    textureImages.push_back(imageBMPLoader.loadImage(appThread->mgr,
//                                                                     "images/cubemap_geo_right.bmp"));
//                    textureImages.push_back(
//                            imageBMPLoader.loadImage(appThread->mgr, "images/cubemap_geo_top.bmp"));
//                    textureImages.push_back(imageBMPLoader.loadImage(appThread->mgr,
//                                                                     "images/cubemap_geo_bottom.bmp"));
//                    break;
//                case EQUIRECTANGULAR_MAP_MODE:
//                    textureImages.push_back(
//                            imageBMPLoader.loadImage(appThread->mgr, "images/tex_current.bmp"));
//                    break;
//            }

            appThread->Application->initialize(mode, textureImages);

            appThread->Started = true;
            appThread->Application->draw();
        }

        if (appThread->Resumed) {
            appThread->Application->draw();
        }

    }

    appThread->Application->stop();

    return NULL;
}

static void ovrAppThread_Create(ovrAppThread *appThread, JNIEnv *env, jobject activity,
                                AAssetManager *assetManager, jstring appDir) {
    appThread->Thread = 0;
    appThread->mgr = assetManager;
    appThread->Resumed = false;
    appThread->Started = false;
    appThread->NativeWindow = NULL;
    appThread->appDir = env->GetStringUTFChars(appDir, JNI_FALSE);
    ovrMessageQueue_Create(&appThread->MessageQueue);

    /* GL Model Factories for the application */
    std::unique_ptr<SceneGLFactory> sceneFactory(new SceneGLFactory(loggerFactory.get()));
    std::unique_ptr<ModelGLFactory> modelFactory(new ModelGLFactory(loggerFactory.get()));

    /* need a separate Camera for OVR due to the way it gives us access to view and projection
     * matrices for each of the eye
     */
    std::unique_ptr<CameraGLOVRFactory> cameraFactory(new CameraGLOVRFactory(loggerFactory.get()));

    /*
     * Separate Texture Factories for OpenGLES - shader language is separate
     */
    std::unique_ptr<DiffuseTextureGLES3Factory> diffuseTextureFactory(
            new DiffuseTextureGLES3Factory(loggerFactory.get()));
    std::unique_ptr<DiffuseTextureCubeMapGLES3Factory> diffuseTextureCubeMapFactory(
            new DiffuseTextureCubeMapGLES3Factory(loggerFactory.get()));

    std::unique_ptr<RendererGearVR> renderer(
            new RendererGearVR(env, activity, loggerFactory.get()));

    std::unique_ptr<TransformCameraFactory> transformCameraFactory(
            new TransformCameraFactory(loggerFactory.get()));
    std::unique_ptr<TransformModelFactory> transformModelFactory(
            new TransformModelFactory(loggerFactory.get()));


    std::unique_ptr<MutexLockAndroid> mutexLock(new MutexLockAndroid);
    eventQueue = std::unique_ptr<IEventQueue>(new EventQueue(std::move(mutexLock)));

    appThread->Application = new Image360(std::move(renderer),
                                    std::move(sceneFactory),
                                    std::move(modelFactory),
                                    std::move(diffuseTextureFactory),
                                    std::move(diffuseTextureCubeMapFactory),
                                    std::move(transformCameraFactory),
                                    std::move(transformModelFactory),
                                    std::move(cameraFactory),
                                    eventQueue.get(),
                                    loggerFactory.get());

    const int createErr = pthread_create(&appThread->Thread, NULL, AppThreadFunction, appThread);
    if (createErr != 0) {
        ALOGE("pthread_create returned %i", createErr);
    }
}

static void ovrAppThread_Destroy(ovrAppThread *appThread, JNIEnv *env) {
    pthread_join(appThread->Thread, NULL);
    appThread->Application->deinitialize();
    appThread->mgr = NULL;
    ovrMessageQueue_Destroy(&appThread->MessageQueue);
}


/*
====================================================================================================

                                         ACTIVITY LIFECYLE

====================================================================================================
*/

JNIEXPORT jlong JNICALL
Java_com_chymeravr_appgearvr_Image360Ad_onCreateNative(JNIEnv *env, jobject obj,
                                                        jobject activity, jobject assetManager,
                                                        jstring appDir) {
    ovrAppThread *appThread = (ovrAppThread *) malloc(sizeof(ovrAppThread));

    loggerFactory = std::unique_ptr<LoggerFactoryAndroid>(new LoggerFactoryAndroid());
    logger = loggerFactory->createLogger("Image360::Native Android");
    logger->log(LOG_DEBUG, "Native Logger Created Successfully");

    auto mgr = AAssetManager_fromJava(env, assetManager);

    ovrAppThread_Create(appThread, env, activity, mgr, appDir);

    ovrMessageQueue_Enable(&appThread->MessageQueue, true);
    ovrMessage message;
    ovrMessage_Init(&message, MESSAGE_ON_CREATE, MQ_WAIT_PROCESSED);
    ovrMessageQueue_PostMessage(&appThread->MessageQueue, &message);

    return (jlong) ((size_t) appThread);

}

JNIEXPORT void JNICALL
Java_com_chymeravr_appgearvr_Image360Ad_onStartNative(JNIEnv *env, jobject obj,
                                                          jobject activity, jlong handle) {

    ovrAppThread *appThread = (ovrAppThread *) ((size_t) handle);
    logger->log(LOG_DEBUG, "onStartNative()");


    ovrMessage message;
    ovrMessage_Init(&message, MESSAGE_ON_START, MQ_WAIT_PROCESSED);
    ovrMessageQueue_PostMessage(&appThread->MessageQueue, &message);
}

JNIEXPORT void JNICALL
Java_com_chymeravr_appgearvr_Image360Ad_onResumeNative(JNIEnv *env, jobject obj, jlong handle) {
    ovrAppThread *appThread = (ovrAppThread *) ((size_t) handle);
    logger->log(LOG_DEBUG, "onResumeNative()");

    ovrMessage message;
    ovrMessage_Init(&message, MESSAGE_ON_RESUME, MQ_WAIT_PROCESSED);
    ovrMessageQueue_PostMessage(&appThread->MessageQueue, &message);
}

JNIEXPORT void JNICALL
Java_com_chymeravr_appgearvr_Image360Ad_onPauseNative(JNIEnv *env, jobject obj, jlong handle) {
    ovrAppThread *appThread = (ovrAppThread *) ((size_t) handle);
    logger->log(LOG_DEBUG, "onPauseNative()");

    ovrMessage message;
    ovrMessage_Init(&message, MESSAGE_ON_PAUSE, MQ_WAIT_PROCESSED);
    ovrMessageQueue_PostMessage(&appThread->MessageQueue, &message);
}

JNIEXPORT void JNICALL
Java_com_chymeravr_appgearvr_Image360Ad_onStopNative(JNIEnv *env, jobject obj, jlong handle) {
    ovrAppThread *appThread = (ovrAppThread *) ((size_t) handle);
    logger->log(LOG_DEBUG, "onStopNative()");

    ovrMessage message;
    ovrMessage_Init(&message, MESSAGE_ON_STOP, MQ_WAIT_PROCESSED);
    ovrMessageQueue_PostMessage(&appThread->MessageQueue, &message);

}

JNIEXPORT void JNICALL
Java_com_chymeravr_appgearvr_Image360Ad_onDestroyNative(JNIEnv *env, jobject obj,
                                                            jlong handle) {
    ovrAppThread *appThread = (ovrAppThread *) ((size_t) handle);
    logger->log(LOG_DEBUG, "onDestroyNative()");

    ovrMessage message;
    ovrMessage_Init(&message, MESSAGE_ON_DESTROY, MQ_WAIT_PROCESSED);
    ovrMessageQueue_PostMessage(&appThread->MessageQueue, &message);
    ovrMessageQueue_Enable(&appThread->MessageQueue, false);

    ovrAppThread_Destroy(appThread, env);
    free(appThread);
}

/*
====================================================================================================

                                         SURFACE LIFECYLE

====================================================================================================
*/

JNIEXPORT void JNICALL
Java_com_chymeravr_appgearvr_Image360Ad_onSurfaceCreatedNative(JNIEnv *env, jobject obj,
                                                                   jlong handle, jobject surface) {
    logger->log(LOG_DEBUG, "onSurfaceCreatedNative() Begin");
    ovrAppThread *appThread = (ovrAppThread *) ((size_t) handle);
    ANativeWindow *newNativeWindow = ANativeWindow_fromSurface(env, surface);

    if (ANativeWindow_getWidth(newNativeWindow) < ANativeWindow_getHeight(newNativeWindow)) {
        // An app that is relaunched after pressing the home button gets an initial surface
        // the orientation even though android:screenOrientation = "landscape" is set in the
        // manifest. The choreographer callback will also never be called for this surface because
        // The surface is immediately replaced with a new surface with the correct orientation.
        logger->log(LOG_ERROR, " Surface not in landscape model! ");
    }
    logger->log(LOG_DEBUG, "       NativeWindow = ANativeWIndow_fromSurface( env, surface )");


    appThread->NativeWindow = newNativeWindow;
    ovrMessage message;
    ovrMessage_Init(&message, MESSAGE_ON_SURFACE_CREATED, MQ_WAIT_PROCESSED);
    ovrMessage_SetPointerParm(&message, 0, appThread->NativeWindow);
    ovrMessageQueue_PostMessage(&appThread->MessageQueue, &message);
}

JNIEXPORT void JNICALL
Java_com_chymeravr_appgearvr_Image360Ad_onSurfaceChangedNative(JNIEnv *env, jobject obj,
                                                                   jlong handle, jobject surface) {
    ovrAppThread *appThread = (ovrAppThread *) ((size_t) handle);
    logger->log(LOG_DEBUG, "onSurfaceChangedNative()");

    ANativeWindow *newNativeWindow = ANativeWindow_fromSurface(env, surface);
    if (ANativeWindow_getWidth(newNativeWindow) < ANativeWindow_getHeight(newNativeWindow)) {
        // An app that is relaunched after pressing the home button gets an initial surface with
        // the wrong orientation even though android:screenOrientation="landscape" is set in the
        // manifest. The choreographer callback will also never be called for this surface because
        // the surface is immediately replaced with a new surface with the correct orientation.
        ALOGE("        Surface not in landscape mode!");
    }

    if (newNativeWindow !=
        appThread->NativeWindow) {
        if (appThread->NativeWindow !=
            NULL) {
            ovrMessage message;
            ovrMessage_Init(&message, MESSAGE_ON_SURFACE_DESTROYED, MQ_WAIT_PROCESSED);
            ovrMessageQueue_PostMessage(&appThread->MessageQueue, &message);
            ALOGV("        ANativeWindow_release( NativeWindow )");
            ANativeWindow_release(appThread->NativeWindow);

        }
        if (newNativeWindow !=
            NULL) {
            ALOGV("         NativeWindow = ANativeWindow_fromSurface( env, surface ) ");
            appThread->NativeWindow = newNativeWindow;
            ovrMessage message;
            ovrMessage_Init(&message, MESSAGE_ON_SURFACE_CREATED, MQ_WAIT_PROCESSED);

            ovrMessage_SetPointerParm(&message, 0, appThread->NativeWindow);
            ovrMessageQueue_PostMessage(&appThread->MessageQueue, &message);
        }
    }
    else if (newNativeWindow != NULL) {
        ANativeWindow_release(newNativeWindow);
    }
}

JNIEXPORT void JNICALL
Java_com_chymeravr_appgearvr_Image360Ad_onSurfaceDestroyedNative(JNIEnv *env, jobject obj,
                                                                     jlong handle) {
    ovrAppThread *appThread = (ovrAppThread *) ((size_t) handle);
    logger->log(LOG_DEBUG, "onSurfaceDestroyedNative()");

    ovrMessage message;
    ovrMessage_Init(&message, MESSAGE_ON_SURFACE_DESTROYED, MQ_WAIT_PROCESSED);
    ovrMessageQueue_PostMessage(&appThread->MessageQueue, &message);
    ANativeWindow_release(appThread->NativeWindow);
    appThread->NativeWindow = NULL;


}


/*
====================================================================================================

                                        INPUT

====================================================================================================
 */

JNIEXPORT void JNICALL
Java_com_chymeravr_appgearvr_Image360Ad_onKeyEventNative(JNIEnv *env, jobject obj, jlong handle,
                                                             int keyCode, int action) {
    if (action == AKEY_EVENT_ACTION_UP) {
        ALOGV("    GLES3JNILib::onKeyEvent( %d, %d )", keyCode, action);
    }
    ovrAppThread *appThread = (ovrAppThread *) ((size_t) handle);
    ovrMessage message;
    ovrMessage_Init(&message, MESSAGE_ON_KEY_EVENT, MQ_WAIT_NONE);
    ovrMessage_SetIntegerParm(&message, 0, keyCode);
    ovrMessage_SetIntegerParm(&message, 1, action);
    ovrMessageQueue_PostMessage(&appThread->MessageQueue, &message);
}

JNIEXPORT void JNICALL
Java_com_chymeravr_appgearvr_Image360Ad_onTouchEventNative(JNIEnv *env, jobject obj,
                                                               jlong handle, int action, float x,
                                                               float y) {
    if (action == AMOTION_EVENT_ACTION_UP) {
        ALOGV("    GLES3JNILib::onTouchEvent( %d, %1.0f, %1.0f )", action, x, y);
    }
    ovrAppThread *appThread = (ovrAppThread *) ((size_t) handle);
    ovrMessage message;
    ovrMessage_Init(&message, MESSAGE_ON_TOUCH_EVENT, MQ_WAIT_NONE);
    ovrMessage_SetIntegerParm(&message, 0, action);
    ovrMessage_SetFloatParm(&message, 1, x);
    ovrMessage_SetFloatParm(&message, 2, y);
    ovrMessageQueue_PostMessage(&appThread->MessageQueue, &message);
}

#ifdef __cplusplus
}
#endif