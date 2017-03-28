//
// Created by robin_chimera on 3/22/2017.
//


#include <android/log.h>
#include <jni.h>

#include <memory>

#include <vr/gvr/capi/include/gvr.h>

// Core Engine modules
#include <coreEngine/events/EventQueue.h>
#include <coreEngine/modifier/ImagePNGLoader.h>
#include <coreEngine/modifier/Image.h>
#include <coreEngine/components/transformTree/TransformTreeFactory.h>
#include <coreEngine/components/gazeDetector/GazeDetectorFactory.h>
#include <coreEngine/factory/IEventGazeListenerFactory.h>
#include <coreEngine/ui/UIFactory.h>
#include <coreEngine/modifier/ImageJPEGLoader.h>

// GLImplementation Modules
#include <glImplementation/factory/SceneGLFactory.h>
#include <glImplementation/factory/ModelGLFactory.h>
#include <glImplementation/factory/opengles2/DiffuseTextureGLES2Factory.h>
#include <glImplementation/factory/opengles2/DiffuseTextureCubeMapGLES2Factory.h>
#include <glImplementation/factory/opengles2/UniformColorFactoryGLES2.h>
#include <glImplementation/factory/opengles2/TextMaterialFactoryGLES2.h>
#include <glImplementation/factory/CameraGLFactory.h>

// Image360 Application
#include <image360/Image360.h>
#include <image360/Image360Stereo.h>

// Android Modules
#include <LoggerGVRFactory.h>
#include <MutexLockGVR.h>
//#include <ImageBMPLoaderAndroid.h>
//#include <RendererOVRM.h>
#include <RendererGVRStereo.h>
#include <GazeListenerFactoryGVR.h>


#define JNI_METHOD(return_type, method_name) \
  JNIEXPORT return_type JNICALL              \
      Java_com_chymeravr_gvradclient_Image360Activity_##method_name

namespace {

    // Logger, Event Queue and Application
    std::unique_ptr<cl::LoggerFactoryGVR> loggerFactory = nullptr;
    std::unique_ptr<cl::ILogger> logger = nullptr;
    std::unique_ptr<cl::IEventQueue> eventQueue = nullptr;
    const char *appDir;
    const char *image360File;
    //bool isRendering = false;

    inline jlong jptr(cl::Image360Stereo *nativeImage360) {
        return reinterpret_cast<intptr_t>(nativeImage360);
    }

    inline cl::Image360Stereo *native(jlong ptr) {
        return reinterpret_cast<cl::Image360Stereo *>(ptr);
    }
}  // anonymous namespace

#ifdef __cplusplus
extern "C" {
#endif



JNI_METHOD(jlong, nativeCreateRenderer)
(JNIEnv *env, jclass clazz, jobject class_loader, jobject android_context,
 jlong native_gvr_api, jstring appDirectory, jstring image360AdFileName) {

    loggerFactory = std::unique_ptr<cl::LoggerFactoryGVR>(new cl::LoggerFactoryGVR());
    logger = loggerFactory->createLogger("Image360::GvrAndroid");
    logger->log(cl::LOG_DEBUG, "Native Logger Created Successfully");

    appDir = env->GetStringUTFChars(appDirectory, JNI_FALSE);
    image360File = env->GetStringUTFChars(image360AdFileName, JNI_FALSE);

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
    std::unique_ptr<cl::DiffuseTextureGLES2Factory> diffuseTextureFactory(
            new cl::DiffuseTextureGLES2Factory(loggerFactory.get()));
    std::unique_ptr<cl::DiffuseTextureCubeMapGLES2Factory> diffuseTextureCubeMapFactory(
            new cl::DiffuseTextureCubeMapGLES2Factory(loggerFactory.get()));

//    std::unique_ptr<cl::RendererGearVR> renderer(
//            new cl::RendererGearVR(env, activity, loggerFactory.get()));
    std::unique_ptr<cl::RendererGVRStereo> renderer(
            new cl::RendererGVRStereo(reinterpret_cast<gvr_context *>(native_gvr_api), loggerFactory.get()));

    std::unique_ptr<cl::MutexLockGVR> mutexLock(new cl::MutexLockGVR);
    eventQueue = std::unique_ptr<cl::IEventQueue>(new cl::EventQueue(std::move(mutexLock)));

    std::unique_ptr<cl::ITransformTreeFactory> transformTreeFactory(
            new cl::TransformTreeFactory(loggerFactory.get()));
    std::unique_ptr<cl::ITransformTreeFactory> uiTransformTreeFactory(
            new cl::TransformTreeFactory(loggerFactory.get()));
    std::unique_ptr<cl::GazeDetectorFactory> gazeDetectorFactory(new cl::GazeDetectorFactory);
    std::unique_ptr<cl::IModelFactory> uiModelFactory(new cl::ModelGLFactory(loggerFactory.get()));
    std::unique_ptr<cl::IUniformColorFactory> uiUniformColorFactory(
            new cl::UniformColorFactoryGLES2(loggerFactory.get()));

    std::unique_ptr<cl::ITextMaterialFactory> textMaterialFactory(
            new cl::TextMaterialFactoryGLES2(loggerFactory.get()));
    std::unique_ptr<cl::UIFactory> uiFactory(
            new cl::UIFactory(loggerFactory.get(), std::move(uiModelFactory),
                              std::move(uiUniformColorFactory),
                              std::move(uiTransformTreeFactory), std::move(textMaterialFactory)));

    std::unique_ptr<cl::IEventGazeListenerFactory> eventGazeListenerFactory(
            new cl::GazeListenerFactoryGVR(loggerFactory.get()));

    std::string absoluteFontFilePath = std::string(appDir) + std::string("/")
                                       + std::string("chymeraSDKAssets/fonts/arial.ttf");
    logger->log(cl::LOG_DEBUG, absoluteFontFilePath);


    return jptr(
            new cl::Image360Stereo(std::move(renderer),
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
                                   absoluteFontFilePath)
    );
}

JNI_METHOD(void, nativeDestroyRenderer)
(JNIEnv *env, jclass clazz, jlong nativeImage360) {
    delete native(nativeImage360);
}

JNI_METHOD(void, nativeInitializeGl)
(JNIEnv *env, jobject obj, jlong nativeImage360) {
    logger->log(cl::LOG_DEBUG, "Initializing Native GL");
    auto image360stereo = native(nativeImage360);
    image360stereo->start();

    cl::ImageJPEGLoader imageJPEGLoader(logger.get());
    std::vector<std::unique_ptr<cl::Image> > textureImages;
    std::string absoluteFilePath = std::string(appDir) + std::string("/")
                                   + std::string(image360File);
    textureImages.push_back(imageJPEGLoader.loadImage(absoluteFilePath));
    //textureImages.push_back(imageBMPLoader.loadImage(absoluteFilePath));

    auto mode = cl::EQUIRECTANGULAR_MAP_MODE;

    image360stereo->initialize(mode, textureImages);
}

JNI_METHOD(void, nativeDrawFrame)
(JNIEnv *env, jobject obj, jlong nativeImage360) {
    //native(nativeImage360)->DrawFrame();
    //if(isRendering) {
        native(nativeImage360)->drawInit();
    //}

//    native(nativeImage360)->draw(cl::LEFT);
//    native(nativeImage360)->draw(cl::RIGHT);
//    native(nativeImage360)->drawComplete();
}

JNI_METHOD(void, nativeOnTriggerEvent)
(JNIEnv *env, jobject obj, jlong nativeImage360) {
    //native(nativeImage360)->OnTriggerEvent();
}

JNI_METHOD(void, nativeOnPause)
(JNIEnv *env, jobject obj, jlong nativeImage360) {
    native(nativeImage360)->pause();
    //gvr_api_->PauseTracking();
    //isRendering = false;
}

JNI_METHOD(void, nativeOnResume)
(JNIEnv *env, jobject obj, jlong nativeImage360) {
    auto image360stereo = native(nativeImage360);
    image360stereo->resume();
    //isRendering = true;
}

#ifdef __cplusplus
}
#endif  // extern "C"

