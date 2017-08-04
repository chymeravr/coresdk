//
// Created by robin_chimera on 7/29/2017.
//

#include "Image360AppWrapper.h"

namespace cl{
    Image360AppWrapper::Image360AppWrapper(JNIEnv *env, jobject image360_activity, jobject android_context,
                                           jlong native_gvr_api, const char *appDir, const char *image360File,
                                           const char *controllerModelFile,
                                           const char *controllerTextureFile,
                                           const char *laserBeamTextureFile){
        // gvr api from google vr sdk
        gvr_api = gvr::GvrApi::WrapNonOwned(reinterpret_cast<gvr_context *>(native_gvr_api));

        // logger util initialization
        this->loggerFactory = std::unique_ptr<LoggerDaydreamFactory>(new LoggerDaydreamFactory());
        this->logger = loggerFactory->createLogger("Image360AppWrapper");
        this->logger->log(LOG_DEBUG, "Native Logger Created Successfully");

        // File & Directory Path initializations
        this->appDir = appDir;
        this->image360File = image360File;
        this->controllerModelFile = controllerModelFile;
        this->controllerTextureFile = controllerTextureFile;
        this->laserBeamTextureFile = laserBeamTextureFile;

        /* GL Factories for the application */
        sceneFactory = std::unique_ptr<SceneGLFactory>(new SceneGLFactory(loggerFactory.get()));
        modelFactory = std::unique_ptr<ModelGLFactory>(new ModelGLFactory(loggerFactory.get()));
        cameraFactory = std::unique_ptr<CameraGLFactory>(new CameraGLFactory(loggerFactory.get()));
        diffuseTextureFactory = std::unique_ptr<DiffuseTextureGLFactory>(
                new DiffuseTextureGLFactory(loggerFactory.get()));
        diffuseTextureCubeMapFactory = std::unique_ptr<DiffuseTextureCubeMapGLFactory>(
                new DiffuseTextureCubeMapGLFactory(loggerFactory.get()));

        transformTreeFactory = std::unique_ptr<ITransformTreeFactory>(
                new TransformTreeFactory(loggerFactory.get()));
        uiTransformTreeFactory = std::unique_ptr<ITransformTreeFactory>(
                new TransformTreeFactory(loggerFactory.get()));
        gazeDetectorFactory = std::unique_ptr<GazeDetectorFactory>(new GazeDetectorFactory);
        uiModelFactory = std::unique_ptr<IModelFactory>(new ModelGLFactory(loggerFactory.get()));
        uiUniformColorFactory = std::unique_ptr<IUniformColorFactory>(
                new UniformColorFactoryGL(loggerFactory.get()));
        uiUniformFadeColorFactory = std::unique_ptr<IUniformFadeColorFactory>(
                new UniformFadeColorFactoryGL(loggerFactory.get()));

        textMaterialFactory = std::unique_ptr<ITextMaterialFactory>(
                new TextMaterialFactoryGL(loggerFactory.get()));
        uiFactory = std::unique_ptr<UIFactory>(
                new UIFactory(loggerFactory.get(), std::move(uiModelFactory),
                                  std::move(uiUniformColorFactory), std::move(uiUniformFadeColorFactory),
                                  std::move(uiTransformTreeFactory), std::move(textMaterialFactory)));

//        eventGazeListenerFactory = std::unique_ptr<IEventGazeListenerFactory>(
//                new GazeListenerFactoryDaydream(loggerFactory.get()));
        eventGazeListenerFactory = std::unique_ptr<IEventGazeListenerFactory>(
                new GazeListenerFactory(loggerFactory.get()));

        std::unique_ptr<IRenderer> renderer = std::unique_ptr<RendererDaydream>(
                new RendererDaydream(gvr_api.get(), loggerFactory.get()));

        std::unique_ptr<IMutexLock> mutexLock(new MutexLockDaydream());
        eventQueue = std::unique_ptr<IEventQueue>(new EventQueue(std::move(mutexLock)));

        this->absoluteFontFilePath = std::string(appDir) + std::string("/")
                               + std::string("chymeraSDKAssets/fonts/arial.ttf");

        this->stereoTextureImages = std::vector<std::unique_ptr<Image>>();

        this->imageJPEGLoader = std::unique_ptr<ImageJPEGLoader>(new ImageJPEGLoader(logger.get()));
        this->imagePNGLoader = std::unique_ptr<ImagePNGLoader>(new ImagePNGLoader(logger.get()));


        this->image360EventClose = std::unique_ptr<IEvent> (new DaydreamActivityCloseEvent(env,
                                                                                           image360_activity,
                                                                                           *loggerFactory));
        this->controllerDaydream = std::unique_ptr<ControllerDaydream>(new ControllerDaydream(gvr_api.get(),
                                                                                              loggerFactory.get()));

        this->image360 = std::unique_ptr<Image360>(new cl::Image360(std::move(renderer),
                                                                    *sceneFactory,
                                                                    *eventQueue, *loggerFactory,
                                                                    *gazeDetectorContainer));
        this->logger->log(LOG_DEBUG, "Image360AppWrapper Created Successfully");
    }
    Image360AppWrapper::~Image360AppWrapper(){}

    void Image360AppWrapper::onStart(){
        logger->log(LOG_DEBUG, "Loading Ad Image");

        gazeDetectorContainer = gazeDetectorFactory->createGazeDetectorContainer();

        std::string controllerTextureAbsoluteFilePath = std::string(this->appDir) + std::string("/")
                                                        + std::string(this->controllerTextureFile);
        std::string laserBeamTextureAbsoluteFilePath = std::string(this->appDir) + std::string("/")
                                                       + std::string(this->laserBeamTextureFile);
        std::string controllerModelAbsoluteFilePath = std::string(this->appDir) + std::string("/")
                                                      + std::string(this->controllerModelFile);


        std::string image360AdAbsoluteFilePath = std::string(this->appDir) + std::string("/")
                                                 + std::string(this->image360File);

        auto image = imageJPEGLoader->loadImage(image360AdAbsoluteFilePath);
        assert(image->data != nullptr);
        this->stereoTextureImages.push_back(std::move(image));

        // stereo sphere initialization
        this->stereoSphere = std::unique_ptr<StereoSphere>(new StereoSphere(
                *this->loggerFactory, *this->modelFactory,
                *this->diffuseTextureFactory, *this->transformTreeFactory,
                std::move(this->stereoTextureImages[0])));

        // controller initialization
        auto controllerTexture = this->imagePNGLoader->loadImage(controllerTextureAbsoluteFilePath);
        auto laserBeamTexture = this->imagePNGLoader->loadImage(laserBeamTextureAbsoluteFilePath);

        assert(controllerTexture->data != nullptr);
        assert(laserBeamTexture != nullptr);

        this->controller = std::unique_ptr<Controller>(new Controller(*this->loggerFactory,
                                                                      *this->modelFactory,
                                                                      *this->transformTreeFactory,
                                                                      *this->diffuseTextureFactory,
                                                                      *this->uiFactory,
                                                                      std::move(controllerTexture),
                                                                      std::move(laserBeamTexture),
                                                                      controllerModelAbsoluteFilePath));

        // fps camera
        this->fpsCamera = std::unique_ptr<FPSCamera>(new FPSCamera(*this->loggerFactory,
                                                                   *this->transformTreeFactory,
                                                                   *this->cameraFactory));

        auto gazeTransformSource = this->controller->getGazeTransformSource();

        this->buttons = std::unique_ptr<Buttons>(new Buttons(*this->loggerFactory, *this->uiFactory,
                                                             *this->gazeDetectorContainer,
                                                             *this->gazeDetectorFactory,
                                                             *this->eventGazeListenerFactory,
                                                             gazeTransformSource,
                                                             absoluteFontFilePath,
                                                             *this->image360EventClose));

        this->buttons->setActionButtonText(std::string("Download"));

        this->controllerClickEvent = std::unique_ptr<ControllerClickEventDaydream>(new ControllerClickEventDaydream(*this->buttons));
        this->controllerDaydream->setControllerClickEventHandler(*this->controllerClickEvent);

        this->image360->addApplicationObject(this->stereoSphere.get());
        this->image360->addApplicationObject(this->fpsCamera.get());
        this->image360->addApplicationObject(this->controller.get());
        this->image360->addApplicationObject(this->buttons.get());
        this->image360->addStereoObject(this->stereoSphere.get());

        this->image360->start();

        this->logger->log(LOG_DEBUG, "Image360AppWrapper Started Successfully");
    }

    void Image360AppWrapper::initialize(){
        this->logger->log(LOG_DEBUG, "Image360AppWrapper Initialize Started");
        auto mode = cl::EQUIRECTANGULAR_MAP_MODE;

        // begin initialization of image 360 application and required components
        this->image360->initialize();

        this->controllerDaydream->setControllerTransform(*(this->controller->getTransformTreeModel()));

        this->logger->log(LOG_DEBUG, "Image360AppWrapper Initialize Finished");
    }

    void Image360AppWrapper::draw(){
        this->controllerDaydream->ProcessControllerInput();
        this->controllerDaydream->updateController();
        this->image360->drawInit();
        this->image360->drawStereoLeft();
        this->image360->drawStereoRight();
        this->image360->drawComplete();
    }

    void Image360AppWrapper::onPause(){
        this->controllerDaydream->pause();
        this->image360->pause();
    }
    void Image360AppWrapper::onResume(){
        this->controllerDaydream->resume();
        this->image360->resume();
    }
    void Image360AppWrapper::onStop(){}
    void Image360AppWrapper::onDestroy(){
        this->image360->deinitialize();
        this->stereoTextureImages.clear();
    }

}