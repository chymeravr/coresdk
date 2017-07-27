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
#include <image360/StereoSphere.h>
#include <image360/Controller.h>
#include <image360/FPSCamera.h>
#include <image360/Buttons.h>
#include <image360/Constants.h>


// Android Modules
#include <LoggerDaydreamFactory.h>
#include <MutexLockDaydream.h>
#include <RendererDaydream.h>
#include <GazeListenerFactoryDaydream.h>
#include <ControllerDaydream.h>
#include "DaydreamActivityCloseEvent.h"


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

    std::unique_ptr<cl::GazeDetectorContainer> gazeDetectorContainer = nullptr;

    // Event Close Applications will be empty here because Daydream can handle fade in fade out on
    // its own
    std::unique_ptr<cl::IEvent> image360EventClose = nullptr;

    std::unique_ptr<cl::StereoSphere> stereoSphere = nullptr;
    std::unique_ptr<cl::Controller> controller = nullptr;
    std::unique_ptr<cl::Buttons> buttons = nullptr;
    std::unique_ptr<cl::FPSCamera> fpsCamera = nullptr;

    //std::vector<std::unique_ptr<cl::Image> > textureImages;

    std::unique_ptr<cl::ImageJPEGLoader> imageJPEGLoader = nullptr;
    std::unique_ptr<cl::ImagePNGLoader> imagePNGLoader = nullptr;

    std::unique_ptr<cl::Image> laserBeamTexture = nullptr;

    std::unique_ptr<cl::Image> controllerTexture = nullptr;

    std::vector<std::unique_ptr<cl::Image> > stereoTextureImages;

    //stereoTextureImages.push_back(imageJPEGLoader.loadImage(stereoEquirectangleImageFilePath));

    // Application components

    // Factories
    std::unique_ptr<cl::SceneGLFactory> sceneFactory = nullptr;
    std::unique_ptr<cl::ModelGLFactory> modelFactory = nullptr;
    std::unique_ptr<cl::CameraGLFactory> cameraFactory = nullptr;
    std::unique_ptr<cl::DiffuseTextureGLFactory> diffuseTextureFactory = nullptr;
    std::unique_ptr<cl::DiffuseTextureCubeMapGLFactory> diffuseTextureCubeMapFactory = nullptr;
    std::unique_ptr<cl::ITransformTreeFactory> transformTreeFactory = nullptr;
    std::unique_ptr<cl::ITransformTreeFactory> uiTransformTreeFactory = nullptr;
    std::unique_ptr<cl::GazeDetectorFactory> gazeDetectorFactory = nullptr;
    std::unique_ptr<cl::IModelFactory> uiModelFactory = nullptr;
    std::unique_ptr<cl::IUniformColorFactory> uiUniformColorFactory = nullptr;
    std::unique_ptr<cl::IUniformFadeColorFactory> uiUniformFadeColorFactory = nullptr;
    std::unique_ptr<cl::ITextMaterialFactory> textMaterialFactory = nullptr;
    std::unique_ptr<cl::UIFactory> uiFactory = nullptr;
    std::unique_ptr<cl::IEventGazeListenerFactory> eventGazeListenerFactory = nullptr;

    // gvr api
    std::unique_ptr<gvr::GvrApi> gvr_api = nullptr;
    // Renderer
    std::unique_ptr<cl::RendererDaydream> renderer = nullptr;
    std::unique_ptr<cl::ControllerDaydream> controllerDaydream = nullptr;

    const char *appDir;
    const char *image360File;
    const char *controllerModelFile;
    const char *controllerTextureFile;
    const char *laserBeamTextureFile;
    std::string absoluteFontFilePath;

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
    sceneFactory = std::unique_ptr<cl::SceneGLFactory>(new cl::SceneGLFactory(loggerFactory.get()));
    modelFactory = std::unique_ptr<cl::ModelGLFactory>(new cl::ModelGLFactory(loggerFactory.get()));

    /* need a separate Camera for OVR due to the way it gives us access to view and projection
     * matrices for each of the eye
     */
//    std::unique_ptr<cl::CameraGLFactory> cameraFactory(
//            new cl::CameraGLFactory(loggerFactory.get()));
    cameraFactory = std::unique_ptr<cl::CameraGLFactory>(new cl::CameraGLFactory(loggerFactory.get()));
    /*
     * Separate Texture Factories for OpenGLES - shader language is separate
     */
    diffuseTextureFactory = std::unique_ptr<cl::DiffuseTextureGLFactory>(
            new cl::DiffuseTextureGLFactory(loggerFactory.get()));
    diffuseTextureCubeMapFactory = std::unique_ptr<cl::DiffuseTextureCubeMapGLFactory>(
            new cl::DiffuseTextureCubeMapGLFactory(loggerFactory.get()));

    gvr_api = gvr::GvrApi::WrapNonOwned(reinterpret_cast<gvr_context *>(native_gvr_api));
    renderer = std::unique_ptr<cl::RendererDaydream>(
//            new cl::RendererDaydream(reinterpret_cast<gvr_context *>(native_gvr_api), loggerFactory.get()));
            new cl::RendererDaydream(gvr_api.get(), loggerFactory.get()));

    std::unique_ptr<cl::MutexLockDaydream> mutexLock(new cl::MutexLockDaydream);
    eventQueue = std::unique_ptr<cl::IEventQueue>(new cl::EventQueue(std::move(mutexLock)));

    transformTreeFactory = std::unique_ptr<cl::ITransformTreeFactory>(
            new cl::TransformTreeFactory(loggerFactory.get()));
    uiTransformTreeFactory = std::unique_ptr<cl::ITransformTreeFactory>(
            new cl::TransformTreeFactory(loggerFactory.get()));
    gazeDetectorFactory = std::unique_ptr<cl::GazeDetectorFactory>(new cl::GazeDetectorFactory);
    uiModelFactory = std::unique_ptr<cl::IModelFactory>(new cl::ModelGLFactory(loggerFactory.get()));
    uiUniformColorFactory = std::unique_ptr<cl::IUniformColorFactory>(
            new cl::UniformColorFactoryGL(loggerFactory.get()));
    uiUniformFadeColorFactory = std::unique_ptr<cl::IUniformFadeColorFactory>(
            new cl::UniformFadeColorFactoryGL(loggerFactory.get()));

    textMaterialFactory = std::unique_ptr<cl::ITextMaterialFactory>(
            new cl::TextMaterialFactoryGL(loggerFactory.get()));
    uiFactory = std::unique_ptr<cl::UIFactory>(
            new cl::UIFactory(loggerFactory.get(), std::move(uiModelFactory),
                              std::move(uiUniformColorFactory), std::move(uiUniformFadeColorFactory),
                              std::move(uiTransformTreeFactory), std::move(textMaterialFactory)));

    eventGazeListenerFactory = std::unique_ptr<cl::IEventGazeListenerFactory>(
            new cl::GazeListenerFactoryDaydream(loggerFactory.get()));

    absoluteFontFilePath = std::string(appDir) + std::string("/")
                                       + std::string("chymeraSDKAssets/fonts/arial.ttf");
    logger->log(cl::LOG_DEBUG, absoluteFontFilePath);

    stereoTextureImages = std::vector<std::unique_ptr<cl::Image>>();

    imageJPEGLoader = std::unique_ptr<cl::ImageJPEGLoader>(new cl::ImageJPEGLoader(logger.get()));
    imagePNGLoader = std::unique_ptr<cl::ImagePNGLoader>(new cl::ImagePNGLoader(logger.get()));

    image360EventClose = std::unique_ptr<cl::IEvent> (new cl::DaydreamActivityCloseEvent(env, &android_context));
    controllerDaydream = std::unique_ptr<cl::ControllerDaydream>(new cl::ControllerDaydream(gvr_api.get(), loggerFactory.get()));
    return jptr(
            new cl::Image360(std::move(renderer), *sceneFactory, *eventQueue, *loggerFactory, *gazeDetectorContainer)
    );
}

JNI_METHOD(void, nativeDestroyRenderer)
(JNIEnv *env, jclass clazz, jlong nativeImage360) {
    logger->log(cl::LOG_DEBUG, "Native Destroy Renderer Start");
    auto image360 = native(nativeImage360);
    image360->stop();
    stereoTextureImages.clear();
    delete native(nativeImage360);
    logger->log(cl::LOG_DEBUG, "Native Destroy Renderer End");
}

JNI_METHOD(void, nativeOnStart)
(JNIEnv *env, jobject obj, jlong nativeImage360) {
    logger->log(cl::LOG_DEBUG, "Starting Image 360 Application");
    auto image360 = native(nativeImage360);

    logger->log(cl::LOG_DEBUG, "Loading Ad Image");

    std::string controllerTextureAbsoluteFilePath = std::string(appDir) + std::string("/")
                                                    + std::string(controllerTextureFile);
    std::string laserBeamTextureAbsoluteFilePath = std::string(appDir) + std::string("/")
                                                   + std::string(laserBeamTextureFile);
    std::string controllerModelAbsoluteFilePath = std::string(appDir) + std::string("/")
                                                  + std::string(controllerModelFile);


    std::string image360AdAbsoluteFilePath = std::string(appDir) + std::string("/")
                                             + std::string(image360File);

    logger->log(cl::LOG_DEBUG, image360AdAbsoluteFilePath);

    auto image = imageJPEGLoader->loadImage(image360AdAbsoluteFilePath);
    assert(image->data != nullptr);
    stereoTextureImages.push_back(std::move(image));


    gazeDetectorContainer = gazeDetectorFactory->createGazeDetectorContainer();

    // stereo sphere initialization
    stereoSphere = std::unique_ptr<cl::StereoSphere>(new cl::StereoSphere(
            *loggerFactory, *modelFactory,
            *diffuseTextureFactory, *transformTreeFactory,
            std::move(stereoTextureImages[0])));

    // controller initialization
    auto controllerTexture = imagePNGLoader->loadImage(controllerTextureAbsoluteFilePath);
    auto laserBeamTexture = imagePNGLoader->loadImage(laserBeamTextureAbsoluteFilePath);

    assert(controllerTexture->data != nullptr);
    assert(laserBeamTexture != nullptr);

    controller = std::unique_ptr<cl::Controller>(new cl::Controller(*loggerFactory, *modelFactory,
                                                                    *transformTreeFactory, *diffuseTextureFactory, *uiFactory,
                                                                    std::move(controllerTexture), std::move(laserBeamTexture),
                                                                    controllerModelAbsoluteFilePath));

    // fps camera
    fpsCamera = std::unique_ptr<cl::FPSCamera>(new cl::FPSCamera(
            *loggerFactory, *transformTreeFactory, *cameraFactory));

//    auto gazeTransformSource = fpsCamera->getCameraTransformTree();
    auto gazeTransformSource = controller->getGazeTransformSource();

    buttons = std::unique_ptr<cl::Buttons>(new cl::Buttons(
            *loggerFactory, *uiFactory,
            *gazeDetectorContainer, *gazeDetectorFactory,
            *eventGazeListenerFactory,
            gazeTransformSource, absoluteFontFilePath, *image360EventClose));
    buttons->setActionButtonText(std::string("Download"));

    image360->addApplicationObject(stereoSphere.get());
    image360->addApplicationObject(fpsCamera.get());
    image360->addApplicationObject(controller.get());
    image360->addApplicationObject(buttons.get());
    image360->addStereoObject(stereoSphere.get());

    image360->start();
    controllerDaydream->ResumeControllerApiAsNeeded();
    logger->log(cl::LOG_DEBUG, "Ad Image Loaded");
}

JNI_METHOD(void, nativeInitializeGl)
(JNIEnv *env, jobject obj, jlong nativeImage360) {
    logger->log(cl::LOG_DEBUG, "Initializing Native GL Start");
    auto image360 = native(nativeImage360);

    auto mode = cl::EQUIRECTANGULAR_MAP_MODE;

    // begin initialization of image 360 application and required components
    image360->initialize();

    ((cl::RendererDaydream *) image360->getRenderer())->setControllerTransform(*(controller->getTransformTreeModel()));
    controllerDaydream->setControllerTransform(*(controller->getTransformTreeModel()));

    logger->log(cl::LOG_DEBUG, "Initializing Native GL Complete");
}

JNI_METHOD(void, nativeDrawFrame)
(JNIEnv *env, jobject obj, jlong nativeImage360) {
    auto image360 = native(nativeImage360);
    controllerDaydream->ProcessControllerInput();
    controllerDaydream->updateController();
    image360->drawInit();
    image360->drawStereoLeft(); //(cl::LEFT);
    image360->drawStereoRight(); //(cl::RIGHT);
    image360->drawComplete();
}

JNI_METHOD(int, nativeOnTriggerEvent)
(JNIEnv *env, jobject obj, jlong nativeImage360) {
    auto image360 = native(nativeImage360);
    logger->log(cl::LOG_DEBUG, "Trigger Event");
//    if(image360->closeButtonListener->inFocus()){
//        logger->log(cl::LOG_DEBUG, "Close Button Clicked");
//        return CLOSE_AD;
//    } else if(image360->actionButtonListener->inFocus()){
//        logger->log(cl::LOG_DEBUG, "Download Button Clicked");
//        return DOWNLOAD;
//    }else{
//        return NO_EVENT;
//    }
    return NO_EVENT;
}

JNI_METHOD(int, nativeOnControllerClicked)
(JNIEnv *env, jobject obj, jlong nativeImage360){
    auto image360 = native(nativeImage360);
    logger->log(cl::LOG_DEBUG, "Trigger Event");
    if(buttons->getCloseButtonListener()->inFocus()){
        logger->log(cl::LOG_DEBUG, "Close Button Clicked");
        return CLOSE_AD;
    } else if(buttons->getActionButtonListener()->inFocus()){
        logger->log(cl::LOG_DEBUG, "Download Button Clicked");
        return DOWNLOAD;
    }

    return NO_EVENT;
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

