#ifndef TESTAPP_TESTAPP_H
#define TESTAPP_TESTAPP_H

#include <coreEngine/IRenderer.h>
#include <coreEngine/IApplication.h>

#include <coreEngine/factory/ISceneFactory.h>
#include <coreEngine/factory/IModelFactory.h>
#include <coreEngine/factory/ITextureFactory.h>
#include <coreEngine/factory/IMaterialDiffuseTextureFactory.h>
#include <coreEngine/factory/IShaderDiffuseTextureFactory.h>
#include <coreEngine/factory/ICameraFactory.h>

#include <coreEngine/components/transform/ITransformCameraFactory.h>
#include <coreEngine/components/transform/ITransformModelFactory.h>

namespace cl{
    class TestApp : public IApplication{
    public:
        TestApp(std::unique_ptr<IRenderer> renderer, std::unique_ptr<ISceneFactory> sceneFactory, std::unique_ptr<IModelFactory> modelFactory, std::unique_ptr<ITextureFactory> textureFactory,
            std::unique_ptr<IMaterialDiffuseTextureFactory> materialDiffuseTextureFactory, std::unique_ptr<IShaderDiffuseTextureFactory> shaderDiffuseTextureFactory,
            std::unique_ptr<ITransformCameraFactory> transformCameraFactory, std::unique_ptr<ITransformModelFactory> transformModelFactory, std::unique_ptr<ICameraFactory> cameraFactory, 
            ILoggerFactory *loggerFactory);

        //IApplication implementation
        void start();
        void initialize();
        void update();
        void draw();
        void deinitialize();
        void stop();

    private:
        std::unique_ptr<IRenderer> renderer;
        std::unique_ptr<ISceneFactory> sceneFactory;
        std::unique_ptr<IModelFactory> modelFactory;
        std::unique_ptr<ITextureFactory> textureFactory;
        std::unique_ptr<IMaterialDiffuseTextureFactory> materialDiffuseTextureFactory;
        std::unique_ptr<IShaderDiffuseTextureFactory> shaderDiffuseTextureFactory;
        std::unique_ptr<ITransformCameraFactory> transformCameraFactory;
        std::unique_ptr<ITransformModelFactory> transformModelFactory;
        std::unique_ptr<ICameraFactory> cameraFactory;
        std::unique_ptr<ILogger> logger;
        std::unique_ptr<Scene> scene;
        Camera *camera;
        ShaderDiffuseTexture *shaderDiffuseTexture;
        MaterialDiffuseTexture *materialDiffuseTexture;
        Texture *imageTexture;
        Model *sphere;
    };
}

#endif //TESTAPP_TESTAPP_H