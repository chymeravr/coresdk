/**
 * Native Activity lifecyle callbacks
 */

#include <jni.h>
#include <android/input.h>


// Core Engine modules
#include <coreEngine/events/EventQueue.h>
#include <coreEngine/modifier/ImagePNGLoader.h>
#include <coreEngine/modifier/Image.h>
#include <coreEngine/components/transformTree/TransformTreeFactory.h>
#include <coreEngine/components/gazeDetector/GazeDetectorFactory.h>
#include <coreEngine/factory/IEventGazeListenerFactory.h>
#include <coreEngine/ui/UIFactory.h>


// GLImplementation Modules
#include <glImplementation/factory/SceneGLFactory.h>
#include <glImplementation/factory/ModelGLFactory.h>
#include <glImplementation/factory/opengles3/DiffuseTextureGLES3Factory.h>
#include <glImplementation/factory/opengles3/DiffuseTextureCubeMapGLES3Factory.h>
#include <glImplementation/factory/opengles3/UniformColorFactoryGLES3.h>
#include <glImplementation/factory/opengles3/TextMaterialFactoryGLES3.h>
#include <glImplementation/factory/CameraGLFactory.h>

// Image360 Application
#include <image360/Image360.h>

// Android Modules
#include <LoggerOVRMFactory.h>
#include <MutexLockOVRM.h>
//#include <ImageBMPLoaderAndroid.h>
//#include <RendererOVRM.h>
#include <RendererOVRMStereo.h>
#include <GazeListenerFactoryOVRM.h>
#include <coreEngine/modifier/ImageJPEGLoader.h>
#include <image360/Image360Stereo.h>

#ifdef __cplusplus
extern "C" {
#endif


//using namespace cl;               // causes error

/*
 * TODO: Get rid of these ugly global variables
 *          move code to CPP 11 standard - use classes instead of
 *          structs etc. etc.
 */

// Logger, Event Queue and Application
std::unique_ptr<cl::LoggerFactoryOVRM> loggerFactory = nullptr;
std::unique_ptr<cl::ILogger> logger = nullptr;
std::unique_ptr<cl::IEventQueue> eventQueue = nullptr;

typedef enum{
    NO_EVENT=0,
    NOTIFY_ME=1,
    CLOSE_AD=2
} keyEventResponse;

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
    } else if (message->Wait == MQ_WAIT_PROCESSED) {
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
    } else if (message->Wait == MQ_WAIT_PROCESSED) {
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
    cl::Image360Stereo *Application;
    jobject Activity;
    const char *appDir;
    const char *appFileName;
    bool Resumed;
    bool Started;
    pthread_t Thread;
    ovrMessageQueue MessageQueue;
    ANativeWindow *NativeWindow;
} ovrAppThread;


void *AppThreadFunction(void *parm) {
    ovrAppThread *appThread = (ovrAppThread *) parm;

    auto renderer = (cl::RendererOVRMStereo *) appThread->Application->getRenderer();

    appThread->Application->start();

    for (bool destroyed = false; destroyed == false;) {
        for (;;) {
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

        if (!appThread->Started && appThread->Resumed) {

            //cl::ImageBMPLoaderAndroid imageBMPLoader(logger.get());
            //cl::ImagePNGLoader imagePNGLoader(logger.get());
            cl::ImageJPEGLoader imageJPEGLoader(logger.get());
            std::vector<std::unique_ptr<cl::Image> > textureImages;
            std::string absoluteFilePath = std::string(appThread->appDir) + std::string("/")
                                           + std::string(appThread->appFileName);
            textureImages.push_back(imageJPEGLoader.loadImage(absoluteFilePath));
            //textureImages.push_back(imageBMPLoader.loadImage(absoluteFilePath));

            cl::TEXTURE_MAP_MODE mode = cl::EQUIRECTANGULAR_MAP_MODE;

//            if((textureImages[0]->width / textureImages[0]->height) != 2) {
//                mode = cl::CUBE_MAP_MODE_SINGLE_IMAGE;
//                logger->log(cl::LOG_DEBUG, "Switching Image format to CubeMap");
//            }

//            switch (mode) {
//                case cl::CUBE_MAP_MODE_SINGLE_IMAGE: {
//                    std::string absoluteFilePath = std::string(appThread->appDir) + std::string("/")
//                                                   + std::string(appThread->appFileName);
//                    textureImages.push_back(imagePNGLoader.loadImage(absoluteFilePath));
//                    break;
//                }
//                case cl::CUBE_MAP_MODE_SIX_IMAGES: {
//                    textureImages.push_back(imageBMPLoader.loadImage(std::string(appThread->appDir)
//                                                                     + std::string(
//                            "/chymeraSDKAssets/image360/cubemap_geo_front.bmp")));
//                    textureImages.push_back(imageBMPLoader.loadImage(std::string(appThread->appDir)
//                                                                     + std::string(
//                            "/chymeraSDKAssets/image360/cubemap_geo_left.bmp")));
//                    textureImages.push_back(imageBMPLoader.loadImage(std::string(appThread->appDir)
//                                                                     + std::string(
//                            "/chymeraSDKAssets/image360/cubemap_geo_back.bmp")));
//                    textureImages.push_back(imageBMPLoader.loadImage(std::string(appThread->appDir)
//                                                                     + std::string(
//                            "/chymeraSDKAssets/image360/cubemap_geo_right.bmp")));
//                    textureImages.push_back(imageBMPLoader.loadImage(std::string(appThread->appDir)
//                                                                     + std::string(
//                            "/chymeraSDKAssets/image360/cubemap_geo_top.bmp")));
//                    textureImages.push_back(imageBMPLoader.loadImage(std::string(appThread->appDir)
//                                                                     + std::string(
//                            "/chymeraSDKAssets/image360/cubemap_geo_bottom.bmp")));
//                    break;
//                }
//                case cl::EQUIRECTANGULAR_MAP_MODE: {
//                    textureImages.push_back(
//                            imageBMPLoader.loadImage(std::string(appThread->appDir)
//                                                     + std::string(
//                                    "/chymeraSDKAssets/image360/cubemap_geo_front.bmp")));
//                    break;
//                }
//            }

            appThread->Application->initialize(mode, textureImages);

            appThread->Started = true;
            appThread->Application->drawInit();
            appThread->Application->draw(cl::LEFT);
            appThread->Application->draw(cl::RIGHT);
            appThread->Application->drawComplete();
        }

        if (appThread->Resumed) {
            appThread->Application->drawInit();
            appThread->Application->draw(cl::LEFT);
            appThread->Application->draw(cl::RIGHT);
            appThread->Application->drawComplete();
        }

    }

    appThread->Application->stop();

    return NULL;
}

static void ovrAppThread_Create(ovrAppThread *appThread, JNIEnv *env, jobject activity,
                                jstring appDir, jstring appFileName, jobject mgr) {
    appThread->Thread = 0;
    appThread->Resumed = false;
    appThread->Started = false;
    appThread->NativeWindow = NULL;
    appThread->appDir = env->GetStringUTFChars(appDir, JNI_FALSE);
    appThread->appFileName = env->GetStringUTFChars(appFileName, JNI_FALSE);
    ovrMessageQueue_Create(&appThread->MessageQueue);

    /* GL Model Factories for the application */
    std::unique_ptr<cl::SceneGLFactory> sceneFactory(new cl::SceneGLFactory(loggerFactory.get()));
    std::unique_ptr<cl::ModelGLFactory> modelFactory(new cl::ModelGLFactory(loggerFactory.get()));

    /* need a separate Camera for OVR due to the way it gives us access to view and projection
     * matrices for each of the eye
     */
    //std::unique_ptr<cl::CameraGLOVRFactory> cameraFactory(new cl::CameraGLOVRFactory(loggerFactory.get()));
    std::unique_ptr<cl::CameraGLFactory> cameraFactory(
            new cl::CameraGLFactory(loggerFactory.get()));
    /*
     * Separate Texture Factories for OpenGLES - shader language is separate
     */
    std::unique_ptr<cl::DiffuseTextureGLES3Factory> diffuseTextureFactory(
            new cl::DiffuseTextureGLES3Factory(loggerFactory.get()));
    std::unique_ptr<cl::DiffuseTextureCubeMapGLES3Factory> diffuseTextureCubeMapFactory(
            new cl::DiffuseTextureCubeMapGLES3Factory(loggerFactory.get()));

//    std::unique_ptr<cl::RendererGearVR> renderer(
//            new cl::RendererGearVR(env, activity, loggerFactory.get()));
    std::unique_ptr<cl::RendererOVRMStereo> renderer(
            new cl::RendererOVRMStereo(env, activity, loggerFactory.get()));

    std::unique_ptr<cl::MutexLockOVRM> mutexLock(new cl::MutexLockOVRM);
    eventQueue = std::unique_ptr<cl::IEventQueue>(new cl::EventQueue(std::move(mutexLock)));

    std::unique_ptr<cl::ITransformTreeFactory> transformTreeFactory(
            new cl::TransformTreeFactory(loggerFactory.get()));
    std::unique_ptr<cl::ITransformTreeFactory> uiTransformTreeFactory(
            new cl::TransformTreeFactory(loggerFactory.get()));
    std::unique_ptr<cl::GazeDetectorFactory> gazeDetectorFactory(new cl::GazeDetectorFactory);
    std::unique_ptr<cl::IModelFactory> uiModelFactory(new cl::ModelGLFactory(loggerFactory.get()));
    std::unique_ptr<cl::IUniformColorFactory> uiUniformColorFactory(
            new cl::UniformColorFactoryGLES3(loggerFactory.get()));

    std::unique_ptr<cl::ITextMaterialFactory> textMaterialFactory(
            new cl::TextMaterialFactoryGLES3(loggerFactory.get()));
    std::unique_ptr<cl::UIFactory> uiFactory(
            new cl::UIFactory(loggerFactory.get(), std::move(uiModelFactory),
                              std::move(uiUniformColorFactory),
                              std::move(uiTransformTreeFactory), std::move(textMaterialFactory)));

    std::unique_ptr<cl::IEventGazeListenerFactory> eventGazeListenerFactory(
            new cl::GazeListenerFactoryOVRM(loggerFactory.get()));

    std::string absoluteFontFilePath = std::string(appThread->appDir) + std::string("/")
                                       + std::string("chymeraSDKAssets/fonts/arial.ttf");
    logger->log(cl::LOG_DEBUG, absoluteFontFilePath);

    appThread->Application = new cl::Image360Stereo(std::move(renderer),
                                              std::move(sceneFactory),
                                              std::move(modelFactory),
                                              std::move(diffuseTextureFactory),
                                              std::move(diffuseTextureCubeMapFactory),
                                              std::move(transformTreeFactory),
                                              std::move(cameraFactory),
                                              eventQueue.get(),
                                              loggerFactory.get(),
                                              std::move(uiFactory),
                                              std::move(gazeDetectorFactory),
                                              std::move(eventGazeListenerFactory),
                                              absoluteFontFilePath);

    const int createErr = pthread_create(&appThread->Thread, NULL, AppThreadFunction, appThread);
    if (createErr != 0) {
        ALOGE("pthread_create returned %i", createErr);
    }
}

static void ovrAppThread_Destroy(ovrAppThread *appThread, JNIEnv *env) {
    pthread_join(appThread->Thread, NULL);
    appThread->Application->deinitialize();
    ovrMessageQueue_Destroy(&appThread->MessageQueue);
}


/*
====================================================================================================

                                         ACTIVITY LIFECYLE

====================================================================================================
*/

JNIEXPORT jlong JNICALL
Java_com_chymeravr_gearvradclient_Image360Activity_onCreateNative(JNIEnv *env, jobject obj,
                                                            jobject activity, jstring appDir,
                                                            jstring appFilename, jobject mgr) {
    ovrAppThread *appThread = (ovrAppThread *) malloc(sizeof(ovrAppThread));

    loggerFactory = std::unique_ptr<cl::LoggerFactoryOVRM>(new cl::LoggerFactoryOVRM());
    logger = loggerFactory->createLogger("Image360::Native Android");
    logger->log(cl::LOG_DEBUG, "Native Logger Created Successfully");

    ovrAppThread_Create(appThread, env, activity, appDir, appFilename, mgr);

    ovrMessageQueue_Enable(&appThread->MessageQueue, true);
    ovrMessage message;
    ovrMessage_Init(&message, MESSAGE_ON_CREATE, MQ_WAIT_PROCESSED);
    ovrMessageQueue_PostMessage(&appThread->MessageQueue, &message);

    return (jlong) ((size_t) appThread);

}

JNIEXPORT void JNICALL
Java_com_chymeravr_gearvradclient_Image360Activity_onStartNative(JNIEnv *env, jobject obj, jlong handle) {
    logger->log(cl::LOG_DEBUG, "onStartNative()");
    ovrAppThread *appThread = (ovrAppThread *) ((size_t) handle);

    ovrMessage message;
    ovrMessage_Init(&message, MESSAGE_ON_START, MQ_WAIT_PROCESSED);
    ovrMessageQueue_PostMessage(&appThread->MessageQueue, &message);
}

JNIEXPORT void JNICALL
Java_com_chymeravr_gearvradclient_Image360Activity_onResumeNative(JNIEnv *env, jobject obj,
                                                            jlong handle) {
    ovrAppThread *appThread = (ovrAppThread *) ((size_t) handle);
    logger->log(cl::LOG_DEBUG, "onResumeNative()");

    ovrMessage message;
    ovrMessage_Init(&message, MESSAGE_ON_RESUME, MQ_WAIT_PROCESSED);
    ovrMessageQueue_PostMessage(&appThread->MessageQueue, &message);
}

JNIEXPORT void JNICALL
Java_com_chymeravr_gearvradclient_Image360Activity_onPauseNative(JNIEnv *env, jobject obj, jlong handle) {
    ovrAppThread *appThread = (ovrAppThread *) ((size_t) handle);
    logger->log(cl::LOG_DEBUG, "onPauseNative()");

    ovrMessage message;
    ovrMessage_Init(&message, MESSAGE_ON_PAUSE, MQ_WAIT_PROCESSED);
    ovrMessageQueue_PostMessage(&appThread->MessageQueue, &message);
}

JNIEXPORT void JNICALL
Java_com_chymeravr_gearvradclient_Image360Activity_onStopNative(JNIEnv *env, jobject obj, jlong handle) {
    ovrAppThread *appThread = (ovrAppThread *) ((size_t) handle);
    logger->log(cl::LOG_DEBUG, "onStopNative()");

    ovrMessage message;
    ovrMessage_Init(&message, MESSAGE_ON_STOP, MQ_WAIT_PROCESSED);
    ovrMessageQueue_PostMessage(&appThread->MessageQueue, &message);

}

JNIEXPORT void JNICALL
Java_com_chymeravr_gearvradclient_Image360Activity_onDestroyNative(JNIEnv *env, jobject obj,
                                                             jlong handle) {
    ovrAppThread *appThread = (ovrAppThread *) ((size_t) handle);
    logger->log(cl::LOG_DEBUG, "onDestroyNative()");

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
Java_com_chymeravr_gearvradclient_Image360Activity_onSurfaceCreatedNative(JNIEnv *env, jobject obj,
                                                                    jlong handle, jobject surface) {
    logger->log(cl::LOG_DEBUG, "onSurfaceCreatedNative() Begin");
    ovrAppThread *appThread = (ovrAppThread *) ((size_t) handle);
    ANativeWindow *newNativeWindow = ANativeWindow_fromSurface(env, surface);

    if (ANativeWindow_getWidth(newNativeWindow) < ANativeWindow_getHeight(newNativeWindow)) {
        // An app that is relaunched after pressing the home button gets an initial surface
        // the orientation even though android:screenOrientation = "landscape" is set in the
        // manifest. The choreographer callback will also never be called for this surface because
        // The surface is immediately replaced with a new surface with the correct orientation.
        logger->log(cl::LOG_ERROR, " Surface not in landscape model! ");
    }
    logger->log(cl::LOG_DEBUG, "       NativeWindow = ANativeWIndow_fromSurface( env, surface )");


    appThread->NativeWindow = newNativeWindow;
    ovrMessage message;
    ovrMessage_Init(&message, MESSAGE_ON_SURFACE_CREATED, MQ_WAIT_PROCESSED);
    ovrMessage_SetPointerParm(&message, 0, appThread->NativeWindow);
    ovrMessageQueue_PostMessage(&appThread->MessageQueue, &message);
}

JNIEXPORT void JNICALL
Java_com_chymeravr_gearvradclient_Image360Activity_onSurfaceChangedNative(JNIEnv *env, jobject obj,
                                                                    jlong handle, jobject surface) {
    ovrAppThread *appThread = (ovrAppThread *) ((size_t) handle);
    logger->log(cl::LOG_DEBUG, "onSurfaceChangedNative()");

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
    } else if (newNativeWindow != NULL) {
        ANativeWindow_release(newNativeWindow);
    }
}

JNIEXPORT void JNICALL
Java_com_chymeravr_gearvradclient_Image360Activity_onSurfaceDestroyedNative(JNIEnv *env, jobject obj,
                                                                      jlong handle) {
    ovrAppThread *appThread = (ovrAppThread *) ((size_t) handle);
    logger->log(cl::LOG_DEBUG, "onSurfaceDestroyedNative()");

    ovrMessage message;
    ovrMessage_Init(&message, MESSAGE_ON_SURFACE_DESTROYED, MQ_WAIT_PROCESSED);
    ovrMessageQueue_PostMessage(&appThread->MessageQueue, &message);
    ANativeWindow_release(appThread->NativeWindow);
    appThread->NativeWindow = NULL;

//    appThread->Started = false;
//    appThread->Application->deinitialize();
}


/*
====================================================================================================

                                        INPUT

====================================================================================================
 */

JNIEXPORT jint JNICALL
Java_com_chymeravr_gearvradclient_Image360Activity_onKeyEventNative(JNIEnv *env, jobject obj,
                                                              jlong handle,
                                                              int keyCode, int action) {
    if (action == AKEY_EVENT_ACTION_UP) {
        ALOGV("    Image360ActivityNative::onKeyEvent( %d, %d )", keyCode, action);
    }
    ovrAppThread *appThread = (ovrAppThread *) ((size_t) handle);
    ovrMessage message;
    ovrMessage_Init(&message, MESSAGE_ON_KEY_EVENT, MQ_WAIT_NONE);
    ovrMessage_SetIntegerParm(&message, 0, keyCode);
    ovrMessage_SetIntegerParm(&message, 1, action);
    ovrMessageQueue_PostMessage(&appThread->MessageQueue, &message);

    if(appThread->Application->closeButtonListener->inFocus()){
        return CLOSE_AD;
    } else if(appThread->Application->actionButtonListener->inFocus()){
        return NOTIFY_ME;
    }else{
        return NO_EVENT;
    }
}

JNIEXPORT int JNICALL
Java_com_chymeravr_gearvradclient_Image360Activity_onTouchEventNative(JNIEnv *env, jobject obj,
                                                                jlong handle, int action, float x,
                                                                float y) {
    if (action == AMOTION_EVENT_ACTION_UP) {
        ALOGV("    Image360ActivityNative::onTouchEvent( %d, %1.0f, %1.0f )", action, x, y);
    }
    ovrAppThread *appThread = (ovrAppThread *) ((size_t) handle);
    ovrMessage message;
    ovrMessage_Init(&message, MESSAGE_ON_TOUCH_EVENT, MQ_WAIT_NONE);
    ovrMessage_SetIntegerParm(&message, 0, action);
    ovrMessage_SetFloatParm(&message, 1, x);
    ovrMessage_SetFloatParm(&message, 2, y);
    ovrMessageQueue_PostMessage(&appThread->MessageQueue, &message);

    if(appThread->Application->closeButtonListener->inFocus()){
        logger->log(cl::LOG_DEBUG, "Close Me Event Detected");
        return CLOSE_AD;
    } else if(appThread->Application->actionButtonListener->inFocus()){
        logger->log(cl::LOG_DEBUG, "Notify Me Event Detected");
        return NOTIFY_ME;
    }else{
        return NO_EVENT;
    }
}

JNIEXPORT jfloatArray JNICALL
Java_com_chymeravr_gearvradclient_Image360Activity_getHMDParamsNative(JNIEnv *env, jobject obj,
                                                                jlong handle) {
    jfloatArray result;
    result = env->NewFloatArray(32);

    ovrAppThread *appThread = (ovrAppThread *) ((size_t) handle);
    auto renderer = (cl::RendererOVRMStereo *) appThread->Application->getRenderer();

    auto hmdParams = renderer->getHMDParams();

    float params[32];
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 16; j++) {
            params[16 * i + j] = hmdParams[16 * i + j];
        }
    }

    env->SetFloatArrayRegion(result, 0, 32, params);
    return result;
}

#ifdef __cplusplus
}
#endif