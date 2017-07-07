//
// Created by robin_chimera on 3/22/2017.
// wires the ad application for daydream headset
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
#include <coreEngine/modifier/ImagePNGLoader.h>

// GLImplementation Modules
#include <glImplementation/factory/SceneGLFactory.h>
#include <glImplementation/factory/ModelGLFactory.h>
#include <glImplementation/factory/DiffuseTextureGLFactory.h>
#include <glImplementation/factory/DiffuseTextureCubeMapGLFactory.h>
#include <glImplementation/factory/UniformColorFactoryGL.h>
#include <glImplementation/factory/UniformFadeColorFactoryGL.h>
#include <glImplementation/factory/TextMaterialFactoryGL.h>
#include <glImplementation/factory/CameraGLFactory.h>

// Image360 Application
#include <image360/Image360.h>
//#include <image360/Image360Mono.h>
//#include <image360/Image360Stereo.h>

// Android Modules
#include <LoggerDaydreamFactory.h>
#include <MutexLockDaydream.h>
#include <RendererDaydream.h>
#include <GazeListenerFactoryDaydream.h>


#define JNI_METHOD(return_type, method_name) \
  JNIEXPORT return_type JNICALL              \
      Java_com_chymeravr_daydreamadclient_Image360Activity_##method_name

namespace {

    typedef enum{
        NO_EVENT=0,
        DOWNLOAD=1,
        CLOSE_AD=2
    } keyEventResponse;

    // Logger, Event Queue and Application
    std::unique_ptr<cl::LoggerDaydreamFactory> loggerFactory = nullptr;
    std::unique_ptr<cl::ILogger> logger = nullptr;
    std::unique_ptr<cl::IEventQueue> eventQueue = nullptr;
    std::vector<std::unique_ptr<cl::Image> > textureImages;

    const char *appDir;
    const char *image360File;
    const char *controllerModelFile;
    const char *controllerTextureFile;
    const char *laserBeamTextureFile;

    inline jlong jptr(cl::Image360 *nativeImage360) {
        return reinterpret_cast<intptr_t>(nativeImage360);
    }

    inline cl::Image360 *native(jlong ptr) {
        return reinterpret_cast<cl::Image360 *>(ptr);
    }


}  // anonymous namespace

#ifdef __cplusplus
extern "C" {
#endif



JNI_METHOD(jlong, nativeCreateRenderer)
(JNIEnv *env, jclass clazz, jobject class_loader, jobject android_context,
 jlong native_gvr_api, jstring appDirectory, jstring image360AdFileName,
 jstring controllerModelFilename, jstring controllerTextureFilename,
 jstring laserBeamTextureFilename) {

    loggerFactory = std::unique_ptr<cl::LoggerDaydreamFactory>(new cl::LoggerDaydreamFactory());
    logger = loggerFactory->createLogger("Image360AdGvrActivityNative");
    logger->log(cl::LOG_DEBUG, "Native Logger Created Successfully");

    appDir = env->GetStringUTFChars(appDirectory, JNI_FALSE);

    // todo: best to move this to the initialization part of the code
    image360File = env->GetStringUTFChars(image360AdFileName, JNI_FALSE);
    controllerModelFile = env->GetStringUTFChars(controllerModelFilename, JNI_FALSE);
    controllerTextureFile = env->GetStringUTFChars(controllerTextureFilename, JNI_FALSE);
    laserBeamTextureFile = env->GetStringUTFChars(laserBeamTextureFilename, JNI_FALSE);

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
    std::unique_ptr<cl::DiffuseTextureGLFactory> diffuseTextureFactory(
            new cl::DiffuseTextureGLFactory(loggerFactory.get()));
    std::unique_ptr<cl::DiffuseTextureCubeMapGLFactory> diffuseTextureCubeMapFactory(
            new cl::DiffuseTextureCubeMapGLFactory(loggerFactory.get()));

    std::unique_ptr<cl::RendererDaydream> renderer(
            new cl::RendererDaydream(reinterpret_cast<gvr_context *>(native_gvr_api), loggerFactory.get()));
//    std::unique_ptr<cl::RendererGVRStereo> renderer(
//            new cl::RendererGVRStereo(reinterpret_cast<gvr_context *>(native_gvr_api), loggerFactory.get()));

    std::unique_ptr<cl::MutexLockDaydream> mutexLock(new cl::MutexLockDaydream);
    eventQueue = std::unique_ptr<cl::IEventQueue>(new cl::EventQueue(std::move(mutexLock)));

    std::unique_ptr<cl::ITransformTreeFactory> transformTreeFactory(
            new cl::TransformTreeFactory(loggerFactory.get()));
    std::unique_ptr<cl::ITransformTreeFactory> uiTransformTreeFactory(
            new cl::TransformTreeFactory(loggerFactory.get()));
    std::unique_ptr<cl::GazeDetectorFactory> gazeDetectorFactory(new cl::GazeDetectorFactory);
    std::unique_ptr<cl::IModelFactory> uiModelFactory(new cl::ModelGLFactory(loggerFactory.get()));
    std::unique_ptr<cl::IUniformColorFactory> uiUniformColorFactory(
            new cl::UniformColorFactoryGL(loggerFactory.get()));
    std::unique_ptr<cl::IUniformFadeColorFactory> uiUniformFadeColorFactory(
            new cl::UniformFadeColorFactoryGL(loggerFactory.get()));

    std::unique_ptr<cl::ITextMaterialFactory> textMaterialFactory(
            new cl::TextMaterialFactoryGL(loggerFactory.get()));
    std::unique_ptr<cl::UIFactory> uiFactory(
            new cl::UIFactory(loggerFactory.get(), std::move(uiModelFactory),
                              std::move(uiUniformColorFactory), std::move(uiUniformFadeColorFactory),
                              std::move(uiTransformTreeFactory), std::move(textMaterialFactory)));

    std::unique_ptr<cl::IEventGazeListenerFactory> eventGazeListenerFactory(
            new cl::GazeListenerFactoryDaydream(loggerFactory.get()));

    std::string absoluteFontFilePath = std::string(appDir) + std::string("/")
                                       + std::string("chymeraSDKAssets/fonts/arial.ttf");
    logger->log(cl::LOG_DEBUG, absoluteFontFilePath);


//    return jptr(
//            new cl::Image360Stereo(std::move(renderer),
//                                   std::move(sceneFactory),
//                                   std::move(modelFactory),
//                                   std::move(diffuseTextureFactory),
//                                   std::move(diffuseTextureCubeMapFactory),
//                                   std::move(transformTreeFactory),
//                                   std::move(cameraFactory),
//                                   eventQueue.get(),
//                                   loggerFactory.get(),
//                                   std::move(uiFactory),
//                                   std::move(gazeDetectorFactory),
//                                   std::move(eventGazeListenerFactory),
//                                   absoluteFontFilePath)
//    );
    return jptr(
            new cl::Image360(std::move(renderer),
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
    logger->log(cl::LOG_DEBUG, "Native Destroy Renderer Start");
    auto image360 = native(nativeImage360);
    image360->stop();
    textureImages.clear();
    delete native(nativeImage360);
    logger->log(cl::LOG_DEBUG, "Native Destroy Renderer End");
}

JNI_METHOD(void, nativeOnStart)
(JNIEnv *env, jobject obj, jlong nativeImage360) {
    logger->log(cl::LOG_DEBUG, "Starting Image 360 Application");
    auto image360 = native(nativeImage360);
    image360->setIsControllerPresent(true);
    image360->start();

    textureImages = std::vector<std::unique_ptr<cl::Image>>();

    logger->log(cl::LOG_DEBUG, "Loading Ad Image");


    cl::ImageJPEGLoader imageJPEGLoader(logger.get());
    std::string image360AdAbsoluteFilePath = std::string(appDir) + std::string("/")
                                   + std::string(image360File);

    //textureImages.push_back(image);
    auto image = imageJPEGLoader.loadImage(image360AdAbsoluteFilePath);
    assert(image->data != nullptr);
    textureImages.push_back(std::move(image));

    logger->log(cl::LOG_DEBUG, "Ad Image Loaded");
}

JNI_METHOD(void, nativeInitializeGl)
(JNIEnv *env, jobject obj, jlong nativeImage360) {
    logger->log(cl::LOG_DEBUG, "Initializing Native GL Start");
    auto image360 = native(nativeImage360);
    //image360->start();
    image360->setActionButtonText(std::string("Download"));

    auto mode = cl::EQUIRECTANGULAR_MAP_MODE;

    cl::ImagePNGLoader imagePNGLoader(logger.get());
    // filePaths that we require to render stuff on screen
    std::string controllerTextureAbsoluteFilePath = std::string(appDir) + std::string("/")
                                   + std::string(controllerTextureFile);
    std::string laserBeamTextureAbsoluteFilePath = std::string(appDir) + std::string("/")
                                                    + std::string(laserBeamTextureFile);
    std::string controllerModelAbsoluteFilePath = std::string(appDir) + std::string("/")
                                                  + std::string(controllerModelFile);

    auto controllerTexture =
            imagePNGLoader.loadImage(controllerTextureAbsoluteFilePath);
    auto laserBeamTexture =
            imagePNGLoader.loadImage(laserBeamTextureAbsoluteFilePath);

    assert(controllerTexture->data != nullptr);
    assert(laserBeamTexture != nullptr);
    logger->log(cl::LOG_DEBUG, controllerModelAbsoluteFilePath);
    // begin initialization of image 360 application and required components
    image360->initialize();

    // stereo texture => stereo view
    image360->initStereoView();
    image360->initStereoEquirectangularView(std::move(textureImages[0]));

    // we require the following UI components
    image360->initCameraReticle();
    image360->initUIButtons();

    image360->initController(std::move(controllerTexture), controllerModelAbsoluteFilePath);
    image360->initControllerLaser(std::move(laserBeamTexture));

    image360->initComplete();
    // end of initialization

    logger->log(cl::LOG_DEBUG, "Initializing Native GL Complete");
}

JNI_METHOD(void, nativeDrawFrame)
(JNIEnv *env, jobject obj, jlong nativeImage360) {
    auto image360 = native(nativeImage360);

    image360->drawInit();
    image360->drawStereoLeft(); //(cl::LEFT);
    image360->drawStereoRight(); //(cl::RIGHT);
    image360->drawComplete();
}

JNI_METHOD(int, nativeOnTriggerEvent)
(JNIEnv *env, jobject obj, jlong nativeImage360) {
    auto image360 = native(nativeImage360);
    logger->log(cl::LOG_DEBUG, "Trigger Event");
    if(image360->closeButtonListener->inFocus()){
        logger->log(cl::LOG_DEBUG, "Close Button Clicked");
        return CLOSE_AD;
    } else if(image360->actionButtonListener->inFocus()){
        logger->log(cl::LOG_DEBUG, "Download Button Clicked");
        return DOWNLOAD;
    }else{
        return NO_EVENT;
    }
}

JNI_METHOD(int, nativeOnControllerClicked)
(JNIEnv *env, jobject obj, jlong nativeImage360){
    auto image360 = native(nativeImage360);
    logger->log(cl::LOG_DEBUG, "Trigger Event");
    if(image360->closeButtonListener->inFocus()){
        logger->log(cl::LOG_DEBUG, "Close Button Clicked");
        return CLOSE_AD;
    } else if(image360->actionButtonListener->inFocus()){
        logger->log(cl::LOG_DEBUG, "Download Button Clicked");
        return DOWNLOAD;
    }else{
        return NO_EVENT;
    }
}

JNI_METHOD(void, nativeOnPause)
(JNIEnv *env, jobject obj, jlong nativeImage360) {
    native(nativeImage360)->pause();

    //isRendering = false;
}

JNI_METHOD(void, nativeOnResume)
(JNIEnv *env, jobject obj, jlong nativeImage360) {
    auto image360 = native(nativeImage360);
    image360->resume();
}

JNI_METHOD(jfloatArray , nativeGetHmdParams)
(JNIEnv *env, jobject obj, jlong nativeImage360){
    // native function that returns quaternion representing current head rotation

    jfloatArray result;
    result = env->NewFloatArray(4);

    auto image360 = native(nativeImage360);

    //todo  : modify renderer to include hmd parameters fetcher as a virtual function
    auto renderer = (cl::RendererDaydream *)image360->getRenderer();

    auto hmdParams = renderer->getHMDParams();

    float params[4];
    for (int j = 0; j < 4; j++) {
        params[j] = hmdParams[j];
    }


    env->SetFloatArrayRegion(result, 0, 4, params);
    return result;
}

#ifdef __cplusplus
}
#endif  // extern "C"

