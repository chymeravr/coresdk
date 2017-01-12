///**
// * Native Surface Holder Callbacks
// */
//
//
//#include <jni.h>
//#include "glesNative.h"
//#include <EGL/egl.h>
//
//#include <android/native_window.h> // requires ndk r5 or newer
//#include <android/native_window_jni.h> // requires ndk r5 or newer
//
//#include <renderer/RendererNoHMD.h>
//#include <RendererGearVR.h>
//#include <coreEngine/factory/ISceneFactory.h>
//#include <glImplementation/factory/SceneGLFactory.h>
//
//#include <MutexLockAndroid.h>
//#include <glImplementation/factory/ModelGLFactory.h>
//#include <coreEngine/factory/IDiffuseTextureFactory.h>
//#include <glImplementation/factory/opengles3/DiffuseTextureGLES3Factory.h>
//#include <glImplementation/factory/opengles3/DiffuseTextureCubeMapGLES3Factory.h>
//#include <coreEngine/components/transform/TransformCameraFactory.h>
//#include <coreEngine/components/transform/TransformModelFactory.h>
//#include <glImplementation/factory/CameraGLFactory.h>
//#include <coreEngine/events/EventQueue.h>
//#include <image360/Image360.h>
////#include
//#include <ImageBMPLoaderAndroid.h>
//
//
//#ifdef __cplusplus
//extern "C" {
//#endif
//using namespace cl;
//
//extern GLESNative *gGlesObject;
//extern std::unique_ptr<LoggerFactoryAndroid> loggerFactory;
//std::unique_ptr<Image360> application;
//std::unique_ptr<IEventQueue> eventQueue = nullptr;
//
//const std::string TAG = "Image360GearVRNative";
//
//
//JNIEXPORT void JNICALL
//Java_com_chymeravr_graphicssdkgearvr_GearVRRenderer_DrawFrameNative(JNIEnv *env,
//                                                                jobject instance) {
//
//    if (gGlesObject == NULL) {
//        return;
//    }
////    gGlesObject->Render();
////    application->draw();
//
//}
//
//JNIEXPORT void JNICALL
//Java_com_chymeravr_graphicssdkgearvr_GearVRRenderer_SurfaceCreatedNative(JNIEnv *env,
//                                                                     jobject instance) {
//
//    if (gGlesObject == NULL) {
//        return;
//    }
//    gGlesObject->PerformGLInits();
//
//    auto logger = loggerFactory->createLogger("TestImageOnAndroid");
//    logger->log(LOG_DEBUG, "Testing logger");
//    std::unique_ptr<SceneGLFactory> sceneFactory(new SceneGLFactory(loggerFactory.get()));
//    std::unique_ptr<ModelGLFactory> modelFactory(new ModelGLFactory(loggerFactory.get()));
//    std::unique_ptr<DiffuseTextureGLES3Factory> diffuseTextureFactory(new DiffuseTextureGLES3Factory(loggerFactory.get()));
//    std::unique_ptr<DiffuseTextureCubeMapGLES3Factory> diffuseTextureCubeMapFactory(new DiffuseTextureCubeMapGLES3Factory(loggerFactory.get()));
////    std::unique_ptr<RendererNoHMD> renderer(new RendererNoHMD());
//    std::unique_ptr<RendererGearVR> renderer(new RendererGearVR(env, instance, loggerFactory.get()));
//    std::unique_ptr<TransformCameraFactory> transformCameraFactory(new TransformCameraFactory(loggerFactory.get()));
//    std::unique_ptr<TransformModelFactory> transformModelFactory(new TransformModelFactory(loggerFactory.get()));
//    std::unique_ptr<CameraGLFactory> cameraFactory(new CameraGLFactory(loggerFactory.get()));
//    std::unique_ptr<MutexLockAndroid> mutexLock(new MutexLockAndroid);
//    eventQueue = std::unique_ptr<IEventQueue>(new EventQueue(std::move(mutexLock)));
//    application = std::unique_ptr<Image360>(new Image360(std::move(renderer),
//                                                         std::move(sceneFactory),
//                                                         std::move(modelFactory),
//                                                         std::move(diffuseTextureFactory),
//                                                         std::move(diffuseTextureCubeMapFactory),
//                                                         std::move(transformCameraFactory),
//                                                         std::move(transformModelFactory),
//                                                         std::move(cameraFactory),
//                                                         eventQueue.get(),
//                                                         loggerFactory.get()));
//
//    // register callbacks
//    application->start();
//
//    ImageBMPLoaderAndroid imageBMPLoader(logger.get());
//    std::vector< std::unique_ptr<Image> > textureImages;
//    TEXTURE_MAP_MODE mode = CUBE_MAP_MODE_SINGLE_IMAGE; //EQUIRECTANGULAR_MAP_MODE;
//
//
//    switch (mode){
//        case CUBE_MAP_MODE_SINGLE_IMAGE:
//            textureImages.push_back(imageBMPLoader.loadImage(gGlesObject->getAAssetManager(), "images/cubemap_current.bmp"));
//            break;
//        case CUBE_MAP_MODE_SIX_IMAGES:
//            textureImages.push_back(imageBMPLoader.loadImage(gGlesObject->getAAssetManager(), "src/main/assets/cubemap_geo_front.bmp"));
//            textureImages.push_back(imageBMPLoader.loadImage(gGlesObject->getAAssetManager(), "src/main/assets/cubemap_geo_left.bmp"));
//            textureImages.push_back(imageBMPLoader.loadImage(gGlesObject->getAAssetManager(), "src/main/assets/cubemap_geo_back.bmp"));
//            textureImages.push_back(imageBMPLoader.loadImage(gGlesObject->getAAssetManager(), "src/main/assets/cubemap_geo_right.bmp"));
//            textureImages.push_back(imageBMPLoader.loadImage(gGlesObject->getAAssetManager(), "src/main/assets/cubemap_geo_top.bmp"));
//            textureImages.push_back(imageBMPLoader.loadImage(gGlesObject->getAAssetManager(), "src/main/assets/cubemap_geo_bottom.bmp"));
//            break;
//        case EQUIRECTANGULAR_MAP_MODE:
//            textureImages.push_back(imageBMPLoader.loadImage(gGlesObject->getAAssetManager(), "src/main/assets/tex_current.bmp"));
//            break;
//    }
//
//    application->initialize(mode, textureImages);
//
//}
//
//JNIEXPORT void JNICALL
//Java_com_chymeravr_graphicssdkgearvr_GearVRRenderer_SurfaceChangedNative(JNIEnv *env,
//                                                                     jobject instance,
//                                                                     jint width,
//                                                                     jint height) {
//
//    if (gGlesObject == NULL) {
//        return;
//    }
//    gGlesObject->SetViewport(width, height);
//
//}
//
//#ifdef __cplusplus
//}
//#endif
//
