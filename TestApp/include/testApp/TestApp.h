#ifndef TESTAPP_TESTAPP_H
#define TESTAPP_TESTAPP_H

#include <coreEngine/IRenderer.h>
#include <coreEngine/IApplication.h>
#include <coreEngine/events/AppEventListener.h>

#include <coreEngine/factory/IModelFactory.h>
#include <coreEngine/factory/ITextureFactory.h>
#include <coreEngine/factory/IMaterialDiffuseTextureFactory.h>
#include <coreEngine/factory/IShaderDiffuseTextureFactory.h>

#include <coreEngine/components/transform/ITransformCameraFactory.h>
#include <coreEngine/components/transform/ITransformModelFactory.h>

namespace cl{
    class TestApp : public AppEventListener, public IApplication{
    public:
        TestApp(std::unique_ptr<IRenderer> renderer, std::unique_ptr<IModelFactory> modelFactory, std::unique_ptr<ITextureFactory> textureFactory,
            std::unique_ptr<IMaterialDiffuseTextureFactory> materialDiffuseTextureFactory, std::unique_ptr<IShaderDiffuseTextureFactory> shaderDiffuseTextureFactory,
            std::unique_ptr<ITransformCameraFactory> transformCameraFactory, std::unique_ptr<ITransformModelFactory> transformModelFactory);

        //AppEventListener implementation
        void onAppStartEvent();
        void onAppPauseEvent();
        void onAppResumeEvent();
        void onAppStopEvent();

        //IApplication implementation
        void start();
        void initialize();
        void update();
        void draw();
        void deinitialize();
        void stop();
    };
}

#endif //TESTAPP_TESTAPP_H