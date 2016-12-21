#include <assert.h>
#include <testApp/TestApp.h>
#include <coreEngine/modifier/TextureBMPLoader.h>
#include <coreEngine/modelBuilder/UVSphereBuilder.h>
#include <coreEngine/modifier/ModelModifier.h>
#include <coreEngine/modelBuilder/CubeBuilder.h>

namespace cl{
    TestApp::TestApp(std::unique_ptr<IRenderer> renderer, std::unique_ptr<ISceneFactory> sceneFactory, std::unique_ptr<IModelFactory> modelFactory, std::unique_ptr<ITextureFactory> textureFactory,
        std::unique_ptr<IMaterialDiffuseTextureFactory> materialDiffuseTextureFactory, std::unique_ptr<IShaderDiffuseTextureFactory> shaderDiffuseTextureFactory,
        std::unique_ptr<ITransformCameraFactory> transformCameraFactory, std::unique_ptr<ITransformModelFactory> transformModelFactory, std::unique_ptr<ICameraFactory> cameraFactory, ILoggerFactory *loggerFactory){
        
        assert(renderer != nullptr);
        assert(sceneFactory != nullptr);
        assert(modelFactory != nullptr);
        assert(textureFactory != nullptr);
        assert(materialDiffuseTextureFactory != nullptr);
        assert(shaderDiffuseTextureFactory != nullptr);
        assert(transformCameraFactory != nullptr);
        assert(transformModelFactory != nullptr);
        assert(cameraFactory != nullptr);
        this->renderer = std::move(renderer);
        this->sceneFactory = std::move(sceneFactory);
        this->modelFactory = std::move(modelFactory);
        this->textureFactory = std::move(textureFactory);
        this->materialDiffuseTextureFactory = std::move(materialDiffuseTextureFactory);
        this->shaderDiffuseTextureFactory = std::move(shaderDiffuseTextureFactory);
        this->transformCameraFactory = std::move(transformCameraFactory);
        this->transformModelFactory = std::move(transformModelFactory);
        this->cameraFactory = std::move(cameraFactory);
        logger = loggerFactory->createLogger("testApp::TestApp: ");
    }

    //IApplication implementation
    void TestApp::start(){
        renderer->start();
        logger->log(LOG_INFO, "Application started.");
    }
    void TestApp::initialize(){
        std::unique_ptr<Camera> camera;
        std::unique_ptr<ShaderDiffuseTexture> shaderDiffuseTexture;
        std::unique_ptr<MaterialDiffuseTexture> materialDiffuseTexture;
        std::unique_ptr<Texture> imageTexture;
        std::unique_ptr<Model> sphere;
        scene = sceneFactory->create("testScene");
        assert(scene != nullptr);
        scene->setBackgroundColor(CL_Vec4(0.0f, 0.0f, 0.4f, 0.0f));
        scene->setDepthTest(true);
        camera = cameraFactory->create("camera", scene.get());
        assert(camera != nullptr);
        camera->setAspect(1.5f);
        camera->setFarPlane(100.0f);
        camera->setFov(1.5f);
        camera->setNearPlane(0.1f);
        this->camera = camera.get();
        scene->addToScene(std::move(camera));

        shaderDiffuseTexture = shaderDiffuseTextureFactory->create("shader", scene.get());
        assert(shaderDiffuseTexture != nullptr);
        this->shaderDiffuseTexture = shaderDiffuseTexture.get();
        scene->addToScene(std::move(shaderDiffuseTexture));

        materialDiffuseTexture = materialDiffuseTextureFactory->create("material", this->shaderDiffuseTexture);
        assert(materialDiffuseTexture != nullptr);
        this->materialDiffuseTexture = materialDiffuseTexture.get();
        scene->addToScene(std::move(materialDiffuseTexture));

        imageTexture = textureFactory->create("imageTexture");
        assert(imageTexture != nullptr);
        TextureBMPLoader textureBMPLoader;
        textureBMPLoader.loadImage(imageTexture.get(), "uvtemplate.bmp");
        this->imageTexture = imageTexture.get();
        scene->addToScene(std::move(imageTexture));

        sphere = modelFactory->create("sphere");
        assert(sphere != nullptr);
        this->sphere = sphere.get();
        scene->addToScene(std::move(sphere));
        
        this->materialDiffuseTexture->setDiffuseTexture(this->imageTexture);

        std::unique_ptr<TransformCamera> transformCameraUptr = transformCameraFactory->create(this->camera);
        this->camera->getComponentList().addComponent(std::move(transformCameraUptr));

        std::unique_ptr<TransformModel> transformSphereUptr= transformModelFactory->create(this->sphere);
        this->sphere->getComponentList().addComponent(std::move(transformSphereUptr));

        TransformCamera *transformCamera = (TransformCamera*)this->camera->getComponentList().getComponent("transform");
        transformCamera->setPosition(CL_Vec3(0.0f, 0.0f, 5.0f));

        TransformModel *transformSphere = (TransformModel*)this->sphere->getComponentList().getComponent("transform");
        transformSphere->setPosition(CL_Vec3(0.0f, 0.0f, 0.0f));

        this->sphere->createBiRelation(this->materialDiffuseTexture);

        std::unique_ptr<ModelModifier> modelModifier(new ModelModifier); 
        CubeBuilder cubeBuilder(modelModifier.get());
        cubeBuilder.buildOutwardCube(this->sphere);
        //UVSphereBuilder uvSphereBuilder(modelModifier.get());
        //uvSphereBuilder.buildUnitSphere(this->sphere, 5);
        //uvSphereBuilder.generateUVMapForAllVertices(this->sphere);

        renderer->initialize(scene.get());
    }
    void TestApp::update(){
        renderer->update();
    }
    void TestApp::draw(){
        renderer->draw(scene.get());
    }
    void TestApp::deinitialize(){
        renderer->deinitialize(scene.get());
    }
    void TestApp::stop(){
        renderer->stop();
    }
}