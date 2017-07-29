//
// Created by robin_chimera on 7/29/2017.
//

#ifndef CVRANDROIDSDK_IMAGE360APPWRAPPER_H
#define CVRANDROIDSDK_IMAGE360APPWRAPPER_H

#include "jni.h"
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
#include <ControllerClickEventDaydream.h>
#include "DaydreamActivityCloseEvent.h"

namespace cl{
    class Image360AppWrapper{
    public:
        Image360AppWrapper(JNIEnv *env, jobject image360_activity, jobject android_context,
                           jlong native_gvr_api,
                           const char *appDir,
                           const char *image360File,
                           const char *controllerModelFile,
                           const char *controllerTextureFile,
                           const char *laserBeamTextureFile);
        ~Image360AppWrapper();
        
        void onStart();
        void initialize();
        void draw();
        void onPause();
        void onResume();
        void onStop();
        void onDestroy();

        std::unique_ptr<Image360> image360 = nullptr;
        
        // Logger, Event Queue and Application
        std::unique_ptr<LoggerDaydreamFactory> loggerFactory = nullptr;
        std::unique_ptr<ILogger> logger = nullptr;
        std::unique_ptr<IEventQueue> eventQueue = nullptr;

        std::unique_ptr<GazeDetectorContainer> gazeDetectorContainer = nullptr;

        // Event Close Applications will be empty here because Daydream can handle fade in fade out on
        // its own
        std::unique_ptr<IEvent> image360EventClose = nullptr;

        std::unique_ptr<StereoSphere> stereoSphere = nullptr;
        std::unique_ptr<Controller> controller = nullptr;
        std::unique_ptr<Buttons> buttons = nullptr;
        std::unique_ptr<FPSCamera> fpsCamera = nullptr;

        std::unique_ptr<ImageJPEGLoader> imageJPEGLoader = nullptr;
        std::unique_ptr<ImagePNGLoader> imagePNGLoader = nullptr;

        std::unique_ptr<Image> laserBeamTexture = nullptr;
        std::unique_ptr<Image> controllerTexture = nullptr;

        std::vector<std::unique_ptr<Image> > stereoTextureImages;

        // Application components

        // Factories
        std::unique_ptr<SceneGLFactory> sceneFactory = nullptr;
        std::unique_ptr<ModelGLFactory> modelFactory = nullptr;
        std::unique_ptr<CameraGLFactory> cameraFactory = nullptr;
        std::unique_ptr<DiffuseTextureGLFactory> diffuseTextureFactory = nullptr;
        std::unique_ptr<DiffuseTextureCubeMapGLFactory> diffuseTextureCubeMapFactory = nullptr;
        std::unique_ptr<ITransformTreeFactory> transformTreeFactory = nullptr;
        std::unique_ptr<ITransformTreeFactory> uiTransformTreeFactory = nullptr;
        std::unique_ptr<GazeDetectorFactory> gazeDetectorFactory = nullptr;
        std::unique_ptr<IModelFactory> uiModelFactory = nullptr;
        std::unique_ptr<IUniformColorFactory> uiUniformColorFactory = nullptr;
        std::unique_ptr<IUniformFadeColorFactory> uiUniformFadeColorFactory = nullptr;
        std::unique_ptr<ITextMaterialFactory> textMaterialFactory = nullptr;
        std::unique_ptr<UIFactory> uiFactory = nullptr;
        std::unique_ptr<IEventGazeListenerFactory> eventGazeListenerFactory = nullptr;

        // gvr api
        std::unique_ptr<gvr::GvrApi> gvr_api = nullptr;
        // Renderer
        std::unique_ptr<ControllerClickEventDaydream> controllerClickEvent = nullptr;
        std::unique_ptr<ControllerDaydream> controllerDaydream = nullptr;

        const char *appDir;
        const char *image360File;
        const char *controllerModelFile;
        const char *controllerTextureFile;
        const char *laserBeamTextureFile;
        std::string absoluteFontFilePath;
    private:
    };
}

#endif //CVRANDROIDSDK_IMAGE360APPWRAPPER_H
