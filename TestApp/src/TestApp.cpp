#include <assert.h>
#include <testApp/TestApp.h>
#include <coreEngine/modifier/TextureBMPLoader.h>
#include <coreEngine/modelBuilder/UVSphereBuilder.h>
#include <coreEngine/modifier/ModelModifier.h>
#include <coreEngine/modelBuilder/CubeBuilder.h>
#include <coreEngine/util/SimpleOBJLoader.h>

namespace cl{
    TestApp::TestApp(std::unique_ptr<IRenderer> renderer, std::unique_ptr<ISceneFactory> sceneFactory, std::unique_ptr<IModelFactory> modelFactory, std::unique_ptr<ITextureFactory> textureFactory,
        std::unique_ptr<IMaterialDiffuseTextureFactory> materialDiffuseTextureFactory, std::unique_ptr<IShaderDiffuseTextureFactory> shaderDiffuseTextureFactory,
        std::unique_ptr<ITransformCameraFactory> transformCameraFactory, std::unique_ptr<ITransformModelFactory> transformModelFactory, std::unique_ptr<ICameraFactory> cameraFactory,
        IEventQueue *eventQueue, ILoggerFactory *loggerFactory){
        
        assert(renderer != nullptr);
        assert(sceneFactory != nullptr);
        assert(modelFactory != nullptr);
        assert(textureFactory != nullptr);
        assert(materialDiffuseTextureFactory != nullptr);
        assert(shaderDiffuseTextureFactory != nullptr);
        assert(transformCameraFactory != nullptr);
        assert(transformModelFactory != nullptr);
        assert(eventQueue != nullptr);
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
        this->eventQueue = eventQueue;
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
        TextureBMPLoader textureBMPLoader(logger.get());
        textureBMPLoader.loadImage(imageTexture.get(), "tex_current.bmp");
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
        transformCamera->setPosition(CL_Vec3(0.0f, 0.0f, 0.0f));
        transformCamera->setRotation(CL_Vec3(0.0f, 0.0f, 0.0f));

        TransformModel *transformSphere = (TransformModel*)this->sphere->getComponentList().getComponent("transform");
        transformSphere->setPosition(CL_Vec3(0.0f, 0.0f, 0.0f));
        
        this->sphere->createBiRelation(this->materialDiffuseTexture);

        std::unique_ptr<ModelModifier> modelModifier(new ModelModifier); 
        //CubeBuilder cubeBuilder(modelModifier.get());
        //cubeBuilder.buildOutwardCube(this->sphere);
        UVSphereBuilder uvSphereBuilder(modelModifier.get());
        uvSphereBuilder.buildUnitSphere(this->sphere, 5);
        //uvSphereBuilder.generateUVMapForAllVertices(this->sphere);
        //SimpleOBJLoader::load("uvsphere.obj", this->sphere);
        //modelModifier->invertNormal(this->sphere);
        //transformSphere->setRotation(CL_Vec3(180.0f, 0.0f, 0.0f));

        /*std::vector<CL_Vec3> &vertices = this->sphere->getVertices();
        std::vector<CL_Vec2> &uvs = this->sphere->getUvs();
        std::vector<CL_GLuint> &indices = this->sphere->getIndices();
        std::string logStr = "\n";
        for (unsigned int i = 0; i < vertices.size(); i++){
            logStr += "<" + std::to_string(vertices[i].x) + "," + std::to_string(vertices[i].y) + "," + std::to_string(vertices[i].z) + ">";
            logStr += "<" + std::to_string(uvs[i].x) + "," + std::to_string(uvs[i].y) + ">\n";
        }
        for (unsigned int i = 0; i < indices.size(); i+=3){
            logStr += std::to_string(indices[i]) + " " + std::to_string(indices[i + 1]) + " " + std::to_string(indices[i + 2]) + "\n";
        }
        logger->log(LOG_DEBUG, logStr);
        */
        renderer->initialize(scene.get());
    }

    void TestApp::update(){
        renderer->update();
    }
    void TestApp::draw(){
        while (!eventQueue->empty()){
            std::unique_ptr<IEvent> event = eventQueue->pop();
            event->callListener();
        }
        renderer->draw(scene.get());
    }
    void TestApp::deinitialize(){
        renderer->deinitialize(scene.get());
    }
    void TestApp::stop(){
        renderer->stop();
    }

    void TestApp::onKeyPress(char key, int x, int y){
        //logger->log(LOG_DEBUG, "Key pressed:" + std::string(1, key));
    }

    void TestApp::onPassiveMouseMotion(int x, int y){
        //logger->log(LOG_DEBUG, "Mouse move:" + std::to_string(x) + "," + std::to_string(y));
        if (lastPassiveMousePositionX != -1){
            float xoff = (x - lastPassiveMousePositionX)*passiveMouseMotionSensitivity;
            float yoff = (lastPassiveMousePositionY - y)*passiveMouseMotionSensitivity;
            Transform *transform = (Transform*)camera->getComponentList().getComponent("transform");
            CL_Vec3 rotation = transform->getRotation();
            transform->setRotation(CL_Vec3(rotation.x+yoff, rotation.y+xoff, rotation.z));
        }
        lastPassiveMousePositionX = x;
        lastPassiveMousePositionY = y;
    }
}