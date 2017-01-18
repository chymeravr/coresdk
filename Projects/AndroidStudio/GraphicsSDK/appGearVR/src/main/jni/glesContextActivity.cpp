/**
 * Native Activity lifecyle callbacks
 */

#include <jni.h>
#include <image360/Image360.h>
#include <LoggerAndroidFactory.h>

#include <RendererGearVR.h>
#include <glImplementation/factory/SceneGLFactory.h>

#include <MutexLockAndroid.h>
#include <glImplementation/factory/ModelGLFactory.h>
#include <glImplementation/factory/opengles3/DiffuseTextureGLES3Factory.h>
#include <glImplementation/factory/opengles3/DiffuseTextureCubeMapGLES3Factory.h>
#include <coreEngine/components/transform/TransformCameraFactory.h>
#include <coreEngine/components/transform/TransformModelFactory.h>
#include <coreEngine/events/EventQueue.h>
#include <ImageBMPLoaderAndroid.h>

#include <thread>
#include <VrApi_Types.h>

#include <CameraGLOVRFactory.h>

#include <android/input.h>

#ifdef __cplusplus
extern "C" {
#endif

using namespace cl;


std::unique_ptr<LoggerFactoryAndroid> loggerFactory = nullptr;
std::unique_ptr<ILogger> logger = nullptr;
std::unique_ptr<Image360> application = nullptr;
AAssetManager *mgr = nullptr;
std::unique_ptr<IEventQueue> eventQueue = nullptr;
pthread_t renderingThread;
static volatile bool isEnabled = false;

void *renderFunc(void *app) {
    Image360 *appl = (Image360 *) app;

    appl->start();
    ImageBMPLoaderAndroid imageBMPLoader(logger.get());
    std::vector<std::unique_ptr<Image> > textureImages;
    TEXTURE_MAP_MODE mode = CUBE_MAP_MODE_SIX_IMAGES; //EQUIRECTANGULAR_MAP_MODE;


    switch (mode) {
        case CUBE_MAP_MODE_SINGLE_IMAGE:
            textureImages.push_back(imageBMPLoader.loadImage(mgr, "images/cubemap_current.bmp"));
            break;
        case CUBE_MAP_MODE_SIX_IMAGES:
            textureImages.push_back(imageBMPLoader.loadImage(mgr, "images/cubemap_geo_front.bmp"));
            textureImages.push_back(imageBMPLoader.loadImage(mgr, "images/cubemap_geo_left.bmp"));
            textureImages.push_back(imageBMPLoader.loadImage(mgr, "images/cubemap_geo_back.bmp"));
            textureImages.push_back(imageBMPLoader.loadImage(mgr, "images/cubemap_geo_right.bmp"));
            textureImages.push_back(imageBMPLoader.loadImage(mgr, "images/cubemap_geo_top.bmp"));
            textureImages.push_back(imageBMPLoader.loadImage(mgr, "images/cubemap_geo_bottom.bmp"));
            break;
        case EQUIRECTANGULAR_MAP_MODE:
            textureImages.push_back(imageBMPLoader.loadImage(mgr, "images/tex_current.bmp"));
            break;
    }

    appl->initialize(mode, textureImages);

    isEnabled = true;
//    application->update();
//    application->draw();
//    application->update();
//    std::thread first (renderThread, application.get());
//    renderingFunc(application);

    while (isEnabled) {
        appl->draw();
    }
    pthread_exit(0);
}


/*
====================================================================================================

                                         ACTIVITY LIFECYLE

====================================================================================================
*/
JNIEXPORT jlong JNICALL
Java_com_chymeravr_appgearvr_ActivityGearVR_onCreateNative(JNIEnv *env, jobject obj,
                                                           jobject activity, jobject assetManager) {
    loggerFactory = std::unique_ptr<LoggerFactoryAndroid>(new LoggerFactoryAndroid());
    logger = loggerFactory->createLogger("Image360::Native Android");
    logger->log(LOG_DEBUG, "Native Logger Created Successfully");

    mgr = AAssetManager_fromJava(env, assetManager);

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
    std::unique_ptr<DiffuseTextureGLES3Factory> diffuseTextureFactory(new DiffuseTextureGLES3Factory(loggerFactory.get()));
    std::unique_ptr<DiffuseTextureCubeMapGLES3Factory> diffuseTextureCubeMapFactory(new DiffuseTextureCubeMapGLES3Factory(loggerFactory.get()));

    std::unique_ptr<RendererGearVR> renderer(new RendererGearVR(env, activity, loggerFactory.get()));

    std::unique_ptr<TransformCameraFactory> transformCameraFactory(new TransformCameraFactory(loggerFactory.get()));
    std::unique_ptr<TransformModelFactory> transformModelFactory(new TransformModelFactory(loggerFactory.get()));


    std::unique_ptr<MutexLockAndroid> mutexLock(new MutexLockAndroid);
    eventQueue = std::unique_ptr<IEventQueue>(new EventQueue(std::move(mutexLock)));

    application = std::unique_ptr<Image360>(new Image360(std::move(renderer),
                                               std::move(sceneFactory),
                                               std::move(modelFactory),
                                               std::move(diffuseTextureFactory),
                                               std::move(diffuseTextureCubeMapFactory),
                                               std::move(transformCameraFactory),
                                               std::move(transformModelFactory),
                                               std::move(cameraFactory),
                                                                    eventQueue.get(),
                                                                    loggerFactory.get()));

    return (jlong)((size_t)application.get());

}

JNIEXPORT void JNICALL
Java_com_chymeravr_appgearvr_ActivityGearVR_onStartNative(JNIEnv *env, jobject obj, jlong handle) {

}

JNIEXPORT void JNICALL
Java_com_chymeravr_appgearvr_ActivityGearVR_onResumeNative(JNIEnv *env, jobject obj, jlong handle) {

    isEnabled = true;
    auto renderer = (RendererGearVR*)application->getRenderer();

    if( renderer->getWindow() != NULL ) {
        const int createErr = pthread_create(&renderingThread, NULL, renderFunc,
                                             (void *) application.get());
        if (createErr != 0) {
            logger->log(LOG_DEBUG, "pthread_create returned errir");
        }
    }
//    appThread->logger->log(LOG_DEBUG, "onResumeNative()");
//    ovrMessage message;
//    ovrMessage_Init(&message, MESSAGE_ON_RESUME, MQ_WAIT_PROCESSED);
//    ovrMessageQueue_PostMessage(&appThread->messageQueue, &message);

}

JNIEXPORT void JNICALL
Java_com_chymeravr_appgearvr_ActivityGearVR_onPauseNative(JNIEnv *env, jobject obj, jlong handle) {
   logger->log(LOG_DEBUG, "onPauseNative()");
    isEnabled = false;

}

JNIEXPORT void JNICALL
Java_com_chymeravr_appgearvr_ActivityGearVR_onStopNative(JNIEnv *env, jobject obj, jlong handle) {
    isEnabled = false;
   application->stop();
}

JNIEXPORT void JNICALL
Java_com_chymeravr_appgearvr_ActivityGearVR_onDestroyNative(JNIEnv *env, jobject obj,
                                                            jlong handle) {
    application->deinitialize();
}

/*
====================================================================================================

                                         SURFACE LIFECYLE

====================================================================================================
*/

JNIEXPORT void JNICALL
Java_com_chymeravr_appgearvr_ActivityGearVR_onSurfaceCreatedNative(JNIEnv *env, jobject obj,
                                                                   jlong handle, jobject surface) {
    logger->log(LOG_DEBUG, "onSurfaceCreatedNative() Begin");
    Image360 *app = (Image360 *)((size_t)handle);

    ANativeWindow *newNativeWindow = ANativeWindow_fromSurface(env, surface);

    if (ANativeWindow_getWidth(newNativeWindow) < ANativeWindow_getHeight(newNativeWindow)) {
        // An app that is relaunched after pressing the home button gets an initial surface
        // the orientation even though android:screenOrientation = "landscape" is set in the
        // manifest. The choreographer callback will also never be called for this surface because
        // The surface is immediately replaced with a new surface with the correct orientation.
        logger->log(LOG_ERROR, " Surface not in landscape model! ");
    }
    logger->log(LOG_DEBUG,
                           "       NativeWindow = ANativeWIndow_fromSurface( env, surface )");
    auto renderer = (RendererGearVR *) application->getRenderer();
    renderer->setWindow(newNativeWindow);

    /*
     * Rendering to Surface must happen on a separate thread - not only is it bad practice not to
     * do so, Oculus VRAPI will not function
     */
    const int createErr = pthread_create( &renderingThread, NULL, renderFunc, (void*) application.get() );
    if (createErr != 0)
    {
        logger->log(LOG_DEBUG, "pthread_create returned errir");
    }
}

JNIEXPORT void JNICALL
Java_com_chymeravr_appgearvr_ActivityGearVR_onSurfaceChangedNative(JNIEnv *env, jobject obj,
                                                                   jlong handle, jobject surface) {
    logger->log(LOG_DEBUG, "onSurfaceChangedNative()");

    ANativeWindow *newNativeWindow = ANativeWindow_fromSurface(env, surface);
    if (ANativeWindow_getWidth(newNativeWindow) < ANativeWindow_getHeight(newNativeWindow)) {
        // An app that is relaunched after pressing the home button gets an initial surface with
        // the wrong orientation even though android:screenOrientation="landscape" is set in the
        // manifest. The choreographer callback will also never be called for this surface because
        // the surface is immediately replaced with a new surface with the correct orientation.
        logger->log(LOG_ERROR, "        Surface not in landscape mode!");
    }
    auto renderer = (RendererGearVR *) application->getRenderer();
    if (newNativeWindow !=  renderer->getWindow())                                                // we have a new window
    {
        if (renderer->getWindow() != NULL)                                                       // there is already a window in this appthread - release it from your slavery !!!
        {
            logger->log(LOG_INFO, "        ANativeWindow_release( NativeWindow )");
            ANativeWindow_release(renderer->getWindow());
            renderer->setWindow(NULL);
        }
        if (newNativeWindow != NULL)                                                               // the new window is not null - wohoO!!
        {
            logger->log(LOG_INFO,
                                   "         NativeWindow = ANativeWindow_fromSurface( env, surface ) ");
            renderer->setWindow(newNativeWindow);
        }
    }
    else if (newNativeWindow != NULL) {
        ANativeWindow_release(newNativeWindow);
    }
}

JNIEXPORT void JNICALL
Java_com_chymeravr_appgearvr_ActivityGearVR_onSurfaceDestroyedNative(JNIEnv *env, jobject obj,
                                                                     jlong handle,
                                                                     jobject surface) {
    isEnabled = false;
    logger->log(LOG_DEBUG, "onSurfaceDestroyedNative()");

    logger->log(LOG_INFO, "        ANativeWindow_release( NativeWindow )");

    auto renderer = (RendererGearVR *) application->getRenderer();
    ANativeWindow_release(renderer->getWindow());
    renderer->setWindow(NULL);
}

#ifdef __cplusplus
}
#endif


/*
====================================================================================================

                                        INPUT

====================================================================================================
 */

JNIEXPORT void JNICALL
Java_com_chymeravr_appgearvr_ActivityGearVR_onKeyEventNative(JNIEnv *env, jobject obj, jlong handle,
                                                             int keyCode, int action) {
}

JNIEXPORT void JNICALL
Java_com_chymeravr_appgearvr_ActivityGearVR_onTouchEventNative(JNIEnv *env, jobject obj,
                                                               jlong handle, int action, float x,
                                                               float y) {
}