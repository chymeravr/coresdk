/**
 * Native Activity lifecyle callbacks
 */

#include <jni.h>
#include <image360/Image360.h>
#include "glesNative.h"

#include <renderer/RendererNoHMD.h>
#include <RendererGearVR.h>
#include <coreEngine/factory/ISceneFactory.h>
#include <glImplementation/factory/SceneGLFactory.h>

#include <MutexLockAndroid.h>
#include <glImplementation/factory/ModelGLFactory.h>
#include <coreEngine/factory/IDiffuseTextureFactory.h>
#include <glImplementation/factory/opengles3/DiffuseTextureGLES3Factory.h>
#include <glImplementation/factory/opengles3/DiffuseTextureCubeMapGLES3Factory.h>
#include <coreEngine/components/transform/TransformCameraFactory.h>
#include <coreEngine/components/transform/TransformModelFactory.h>
#include <glImplementation/factory/CameraGLFactory.h>
#include <coreEngine/events/EventQueue.h>
#include <image360/Image360.h>
#include <RendererGearVR.h>
#include <ImageBMPLoaderAndroid.h>

#include <thread>
#include <VrApi_Types.h>

#include "CameraGLOVRFactory.h"

#ifdef __cplusplus
extern "C" {
#endif

using namespace cl;

GLESNative *gGlesObject = nullptr;
//LoggerFactoryAndroid* loggerFactory;
std::unique_ptr<LoggerFactoryAndroid> loggerFactory = nullptr;
std::unique_ptr<ILogger> logger = nullptr;
//std::unique_ptr<Image360> application = nullptr;
Image360* application = nullptr;
std::unique_ptr<IEventQueue> eventQueue = nullptr;
pthread_t renderingThread;

static volatile bool isEnabled = false;

void* renderFunc(void* app){
    Image360* appl = (Image360*) app;

    appl->start();
    ImageBMPLoaderAndroid imageBMPLoader(logger.get());
    std::vector< std::unique_ptr<Image> > textureImages;
    TEXTURE_MAP_MODE mode = CUBE_MAP_MODE_SINGLE_IMAGE; //EQUIRECTANGULAR_MAP_MODE;


    switch (mode){
        case CUBE_MAP_MODE_SINGLE_IMAGE:
            textureImages.push_back(imageBMPLoader.loadImage(gGlesObject->getAAssetManager(), "images/cubemap_current.bmp"));
            break;
        case CUBE_MAP_MODE_SIX_IMAGES:
            textureImages.push_back(imageBMPLoader.loadImage(gGlesObject->getAAssetManager(), "src/main/assets/cubemap_geo_front.bmp"));
            textureImages.push_back(imageBMPLoader.loadImage(gGlesObject->getAAssetManager(), "src/main/assets/cubemap_geo_left.bmp"));
            textureImages.push_back(imageBMPLoader.loadImage(gGlesObject->getAAssetManager(), "src/main/assets/cubemap_geo_back.bmp"));
            textureImages.push_back(imageBMPLoader.loadImage(gGlesObject->getAAssetManager(), "src/main/assets/cubemap_geo_right.bmp"));
            textureImages.push_back(imageBMPLoader.loadImage(gGlesObject->getAAssetManager(), "src/main/assets/cubemap_geo_top.bmp"));
            textureImages.push_back(imageBMPLoader.loadImage(gGlesObject->getAAssetManager(), "src/main/assets/cubemap_geo_bottom.bmp"));
            break;
        case EQUIRECTANGULAR_MAP_MODE:
            textureImages.push_back(imageBMPLoader.loadImage(gGlesObject->getAAssetManager(), "src/main/assets/tex_current.bmp"));
            break;
    }

    appl->initialize(mode, textureImages);

    isEnabled = true;
//    application->update();
//    application->draw();
//    application->update();
//    std::thread first (renderThread, application.get());
//    renderingFunc(application);

    while(true) {
//        app->update();
        appl->draw();
    }
}

//void renderingFunc(Image360* app)
//{
//    app->draw();
//}


/*
====================================================================================================

                                         ACTIVITY LIFECYLE

====================================================================================================
*/
JNIEXPORT jlong JNICALL
Java_com_chymeravr_appgearvr_ActivityGearVR_onCreateNative(JNIEnv *env, jobject obj,
                                                                   jobject activity, jobject assetManager) {
    std::string tag = "GLESNative";
    loggerFactory = std::unique_ptr<LoggerFactoryAndroid>(new LoggerFactoryAndroid());
    logger = loggerFactory->createLogger("Image360::Native Android");
    logger->log(LOG_DEBUG, "Native Logger Created Successfully");

    AAssetManager* mgr = AAssetManager_fromJava(env, assetManager);
    gGlesObject = new GLESNative(loggerFactory.get(), mgr);

    std::unique_ptr<SceneGLFactory> sceneFactory(new SceneGLFactory(loggerFactory.get()));
    std::unique_ptr<ModelGLFactory> modelFactory(new ModelGLFactory(loggerFactory.get()));
    std::unique_ptr<DiffuseTextureGLES3Factory> diffuseTextureFactory(new DiffuseTextureGLES3Factory(loggerFactory.get()));
    std::unique_ptr<DiffuseTextureCubeMapGLES3Factory> diffuseTextureCubeMapFactory(new DiffuseTextureCubeMapGLES3Factory(loggerFactory.get()));
    //std::unique_ptr<RendererNoHMD> renderer(new RendererNoHMD());
    std::unique_ptr<RendererGearVR> renderer(new RendererGearVR(env, activity, loggerFactory.get()));
    std::unique_ptr<TransformCameraFactory> transformCameraFactory(new TransformCameraFactory(loggerFactory.get()));
    std::unique_ptr<TransformModelFactory> transformModelFactory(new TransformModelFactory(loggerFactory.get()));
//    std::unique_ptr<CameraGLFactory> cameraFactory(new CameraGLFactory(loggerFactory.get()));
    std::unique_ptr<CameraGLOVRFactory> cameraFactory(new CameraGLOVRFactory(loggerFactory.get()));
    std::unique_ptr<MutexLockAndroid> mutexLock(new MutexLockAndroid);
    eventQueue = std::unique_ptr<IEventQueue>(new EventQueue(std::move(mutexLock)));
    application = new Image360(std::move(renderer),
                                 std::move(sceneFactory),
                                 std::move(modelFactory),
                                 std::move(diffuseTextureFactory),
                                 std::move(diffuseTextureCubeMapFactory),
                                 std::move(transformCameraFactory),
                                 std::move(transformModelFactory),
                                 std::move(cameraFactory),
                                 eventQueue.get(),
                                 loggerFactory.get());

    return (jlong)((size_t)application);

}

JNIEXPORT void JNICALL
Java_com_chymeravr_appgearvr_ActivityGearVR_onStartNative(JNIEnv *env, jobject obj, jlong handle)
{
    logger->log(LOG_DEBUG, "onStartNative()");


}

JNIEXPORT void JNICALL
Java_com_chymeravr_appgearvr_ActivityGearVR_onResumeNative(JNIEnv *env, jobject obj, jlong handle)
{
    logger->log(LOG_DEBUG, "onResumeNative()");

}

JNIEXPORT void JNICALL
Java_com_chymeravr_appgearvr_ActivityGearVR_onPauseNative(JNIEnv *env, jobject obj, jlong handle)
{
    logger->log(LOG_DEBUG, "onPauseNative()");

}

JNIEXPORT void JNICALL
Java_com_chymeravr_appgearvr_ActivityGearVR_onStopNative(JNIEnv *env, jobject obj, jlong handle)
{
    logger->log(LOG_DEBUG, "onStopNative()");
    application->stop();
}

JNIEXPORT void JNICALL
Java_com_chymeravr_appgearvr_ActivityGearVR_onDestroyNative(JNIEnv *env, jobject obj, jlong handle)
{
    logger->log(LOG_DEBUG, "onDestroyNative() - Cleaning up stuff!");
    application->deinitialize();
}

/*
====================================================================================================

                                         SURFACE LIFECYLE

====================================================================================================
*/

JNIEXPORT void JNICALL
Java_com_chymeravr_appgearvr_ActivityGearVR_onSurfaceCreatedNative(JNIEnv * env, jobject obj,
                                                                           jlong handle, jobject surface)
{
    logger->log(LOG_DEBUG, "onSurfaceCreatedNative() Begin");
    Image360 *app = (Image360 *)((size_t)handle);
//    ovrAppThread * appThread = (ovrAppThread *)((size_t)handle);
//    if (gGlesObject == NULL) {
//        return;
//    }
//    gGlesObject->PerformGLInits();
    ANativeWindow* newNativeWindow = ANativeWindow_fromSurface(env, surface);

    if ( ANativeWindow_getWidth( newNativeWindow ) < ANativeWindow_getHeight( newNativeWindow))
    {
        // An app that is relaunched after pressing the home button gets an initial surface
        // the orientation even though android:screenOrientation = "landscape" is set in the
        // manifest. The choreographer callback will also never be called for this surface because
        // The surface is immediately replaced with a new surface with the correct orientation.
        logger->log(LOG_ERROR, " Surface not in landscape model! ");
    }
    logger->log( LOG_DEBUG, "       NativeWindow = ANativeWIndow_fromSurface( env, surface )" );
    auto renderer = (RendererGearVR*)app->getRenderer();
    renderer->setWindow(newNativeWindow);


    const int createErr = pthread_create( &renderingThread, NULL, renderFunc, (void*) app );
    if (createErr != 0)
    {
        logger->log(LOG_DEBUG, "pthread_create returned errir");
    }
//    renderThread((void*) application.get());

    logger->log(LOG_DEBUG, "onSurfaceCreatedNative() Completed");
}

JNIEXPORT void JNICALL
Java_com_chymeravr_appgearvr_ActivityGearVR_onSurfaceChangedNative(JNIEnv * env, jobject obj,
                                                                           jlong handle, jobject surface)
{
    logger->log(LOG_DEBUG, "onSurfaceChangedNative()");
}

JNIEXPORT void JNICALL
Java_com_chymeravr_appgearvr_ActivityGearVR_onSurfaceDestroyedNative(JNIEnv * env, jobject obj,
                                                                           jlong handle, jobject surface)
{
    isEnabled = false;
    logger->log(LOG_DEBUG, "onSurfaceDestroyedNative()");
}

#ifdef __cplusplus
}
#endif
