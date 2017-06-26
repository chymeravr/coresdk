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

// Android Modules
#include <LoggerCardboardFactory.h>
#include <MutexLockCardboard.h>
#include <RendererCardboard.h>
#include <CloseButtonListenerCardboard.h>
#include <GazeListenerFactoryCardboard.h>


#define JNI_METHOD(return_type, method_name) \
  JNIEXPORT return_type JNICALL              \
      Java_com_chymeravr_cardboardadclient_Image360Activity_##method_name

namespace {

    typedef enum{
        NO_EVENT=0,
        DOWNLOAD=1,
        CLOSE_AD=2
    } keyEventResponse;

    // Logger, Event Queue and Application
    std::unique_ptr<cl::LoggerCardboardFactory> loggerFactory = nullptr;
    std::unique_ptr<cl::ILogger> logger = nullptr;
    std::unique_ptr<cl::IEventQueue> eventQueue = nullptr;
    std::vector<std::unique_ptr<cl::Image> > textureImages;

    const char *appDir;
    const char *image360File;

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
 jlong native_gvr_api, jstring appDirectory, jstring image360AdFileName) {

    loggerFactory = std::unique_ptr<cl::LoggerCardboardFactory>(new cl::LoggerCardboardFactory());
    logger = loggerFactory->createLogger("Image360AdCardboardActivityNative");
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
    std::unique_ptr<cl::DiffuseTextureGLFactory> diffuseTextureFactory(
            new cl::DiffuseTextureGLFactory(loggerFactory.get()));
    std::unique_ptr<cl::DiffuseTextureCubeMapGLFactory> diffuseTextureCubeMapFactory(
            new cl::DiffuseTextureCubeMapGLFactory(loggerFactory.get()));

    std::unique_ptr<cl::RendererCardboard> renderer(
            new cl::RendererCardboard(reinterpret_cast<gvr_context *>(native_gvr_api), loggerFactory.get()));
//    std::unique_ptr<cl::RendererGVRStereo> renderer(
//            new cl::RendererGVRStereo(reinterpret_cast<gvr_context *>(native_gvr_api), loggerFactory.get()));

    std::unique_ptr<cl::MutexLockCardboard> mutexLock(new cl::MutexLockCardboard);
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
            new cl::GazeListenerFactoryCardboard(loggerFactory.get()));

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
    image360->deinitialize();
    textureImages.clear();
    delete native(nativeImage360);
    logger->log(cl::LOG_DEBUG, "Native Destroy Renderer End");
}

//JNI_METHOD(void, nativeOnStop)
//(JNIEnv *env, jclass clazz, jlong nativeImage360) {
//    logger->log(cl::LOG_DEBUG, "Native On Image360 Application Stop");
//    auto image360 = native(nativeImage360);
//    image360->stop();
//}

JNI_METHOD(void, nativeOnStart)
(JNIEnv *env, jobject obj, jlong nativeImage360) {
    logger->log(cl::LOG_DEBUG, "Starting Image 360 Application");
    auto image360 = native(nativeImage360);
    image360->setIsControllerPresent(false);
    image360->start();

    textureImages = std::vector<std::unique_ptr<cl::Image>>();

    logger->log(cl::LOG_DEBUG, "Loading Ad Image");


    cl::ImageJPEGLoader imageJPEGLoader(logger.get());
    std::string absoluteFilePath = std::string(appDir) + std::string("/")
                                   + std::string(image360File);
    //textureImages.push_back(image);
    auto image = imageJPEGLoader.loadImage(absoluteFilePath);
    textureImages.push_back(std::move(image));

    logger->log(cl::LOG_DEBUG, "Ad Image Loaded");
}

JNI_METHOD(void, nativeInitializeGl)
(JNIEnv *env, jobject obj, jlong nativeImage360) {
    logger->log(cl::LOG_DEBUG, "Initializing Native GL Start");
    auto image360 = native(nativeImage360);
    //image360->start();
    image360->setActionButtonText(std::string("Notify Me"));

    auto mode = cl::EQUIRECTANGULAR_MAP_MODE;

    //image360->initialize(mode, textureImages);
    image360->initialize();
    image360->initStereoView();
    image360->initStereoEquirectangularView(std::move(textureImages[0]));

    image360->initCameraReticle();
    image360->initUIButtons();
    image360->initFadeScreen();     // fade screen is required in cardboard
    image360->initComplete();

    logger->log(cl::LOG_DEBUG, "Initializing Native GL Complete");
}

JNI_METHOD(void, nativeDrawFrame)
(JNIEnv *env, jobject obj, jlong nativeImage360) {
    auto image360 = native(nativeImage360);

    image360->drawInit();
    image360->drawStereoLeft();
    image360->drawStereoRight();
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
}

JNI_METHOD(void, nativeOnResume)
(JNIEnv *env, jobject obj, jlong nativeImage360) {
    auto image360 = native(nativeImage360);
    image360->resume();
}

JNI_METHOD(jboolean , nativeCloseAd)
(JNIEnv *env, jobject obj, jlong nativeImage360) {
    auto image360 = native(nativeImage360);
    auto closeButtonListener = (cl::CloseButtonListenerCardboard*) image360->closeButtonListener.get();
    if(closeButtonListener->isLongGaze()){
        image360->beginFade();
    }
    return image360->isFadeComplete();
}

JNI_METHOD(jfloatArray , nativeGetHmdParams)
(JNIEnv *env, jobject obj, jlong nativeImage360){
    // native function that returns quaternion representing current head rotation

    jfloatArray result;
    result = env->NewFloatArray(4);

    auto image360 = native(nativeImage360);

    //todo  : modify renderer to include hmd parameters fetcher as a virtual function
    auto renderer = (cl::RendererCardboard *)image360->getRenderer();

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

