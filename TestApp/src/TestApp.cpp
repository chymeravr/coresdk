#include <assert.h>
#include <testApp/TestApp.h>
#include <coreEngine/modelBuilder/UVSphereBuilder.h>
#include <coreEngine/modifier/ModelModifier.h>
#include <coreEngine/modelBuilder/CubeBuilder.h>
#include <coreEngine/util/SimpleOBJLoader.h>
#include <coreEngine/modifier/ImageModifier.h>

namespace cl{
    TestApp::TestApp(std::unique_ptr<IRenderer> renderer, std::unique_ptr<ISceneFactory> sceneFactory, std::unique_ptr<IModelFactory> modelFactory, std::unique_ptr<IDiffuseTextureFactory> diffuseTextureFactory,
        std::unique_ptr<IDiffuseTextureCubeMapFactory> diffuseTextureCubeMapFactory,
        std::unique_ptr<ITransformCameraFactory> transformCameraFactory, std::unique_ptr<ITransformModelFactory> transformModelFactory, std::unique_ptr<ICameraFactory> cameraFactory,
        IEventQueue *eventQueue, ILoggerFactory *loggerFactory){
        
        assert(renderer != nullptr);
        assert(sceneFactory != nullptr);
        assert(modelFactory != nullptr);
        assert(diffuseTextureFactory != nullptr);
        assert(diffuseTextureCubeMapFactory != nullptr);
        assert(transformCameraFactory != nullptr);
        assert(transformModelFactory != nullptr);
        assert(eventQueue != nullptr);
        assert(cameraFactory != nullptr);
        this->renderer = std::move(renderer);
        this->sceneFactory = std::move(sceneFactory);
        this->modelFactory = std::move(modelFactory);
        this->diffuseTextureFactory = std::move(diffuseTextureFactory);
        this->diffuseTextureCubeMapFactory = std::move(diffuseTextureCubeMapFactory);
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
    void TestApp::initialize(TEXTURE_MAP_MODE mapMode, std::vector<std::unique_ptr<Image>> &textureImages){
        std::unique_ptr<Camera> camera;
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

        std::unique_ptr<Model> imageContainer = modelFactory->create("imageContainer");
        assert(imageContainer != nullptr);
        this->imageContainer = imageContainer.get();
        scene->addToScene(std::move(imageContainer));

        if (mapMode == EQUIRECTANGULAR_MAP_MODE){
            std::unique_ptr<ShaderDiffuseTexture> shaderDiffuseTexture;
            std::unique_ptr<MaterialDiffuseTexture> materialDiffuseTexture;
            std::unique_ptr<Texture> imageTexture;
            shaderDiffuseTexture = diffuseTextureFactory->createShader("shader", scene.get());
            assert(shaderDiffuseTexture != nullptr);
            this->shader = shaderDiffuseTexture.get();
            scene->addToScene(std::move(shaderDiffuseTexture));

            materialDiffuseTexture = diffuseTextureFactory->createMaterial("material", (ShaderDiffuseTexture*)this->shader);
            assert(materialDiffuseTexture != nullptr);
            this->material = materialDiffuseTexture.get();
            scene->addToScene(std::move(materialDiffuseTexture));

            imageTexture = diffuseTextureFactory->createTexture("imageTexture");
            assert(imageTexture != nullptr);
            assert(textureImages.size() == 1);
            assert(textureImages[0] != nullptr);
            std::unique_ptr<Image> image = std::move(textureImages[0]);
            imageTexture->setTextureData(std::move(image->data));
            imageTexture->setHeight(image->height);
            imageTexture->setWidth(image->width);
            imageTexture->setTextureDataSize(image->dataSize);
            this->imageTexture = imageTexture.get();
            scene->addToScene(std::move(imageTexture));
            ((MaterialDiffuseTexture*)this->material)->setDiffuseTexture(this->imageTexture);

            std::unique_ptr<TransformCamera> transformCameraUptr = transformCameraFactory->create(this->camera);
            this->camera->getComponentList().addComponent(std::move(transformCameraUptr));

            std::unique_ptr<TransformModel> transformSphereUptr = transformModelFactory->create(this->imageContainer);
            this->imageContainer->getComponentList().addComponent(std::move(transformSphereUptr));

            TransformCamera *transformCamera = (TransformCamera*)this->camera->getComponentList().getComponent("transform");
            transformCamera->setPosition(CL_Vec3(0.0f, 0.0f, 0.0f));
            transformCamera->setRotation(CL_Vec3(0.0f, 0.0f, 0.0f));

            TransformModel *transformSphere = (TransformModel*)this->imageContainer->getComponentList().getComponent("transform");
            transformSphere->setPosition(CL_Vec3(0.0f, 0.0f, 0.0f));

            this->imageContainer->createBiRelation(this->material);

            std::unique_ptr<ModelModifier> modelModifier(new ModelModifier);
            UVSphereBuilder uvSphereBuilder(modelModifier.get());
            uvSphereBuilder.buildUnitSphere(this->imageContainer, 5);
            //uvSphereBuilder.generateUVMapForAllVertices(this->imageContainer);
            //SimpleOBJLoader::load("uvsphere.obj", this->imageContainer);
            //modelModifier->invertNormal(this->imageContainer);
            //transformSphere->setRotation(CL_Vec3(180.0f, 0.0f, 0.0f));
        }
        else{
            std::unique_ptr<ShaderDiffuseTextureCubeMap> shaderUPtr = diffuseTextureCubeMapFactory->createShader("shader", scene.get());
            assert(shaderUPtr != nullptr);
            this->shader = shaderUPtr.get();
            scene->addToScene(std::move(shaderUPtr));

            std::unique_ptr<MaterialDiffuseTextureCubeMap> materialUPtr = diffuseTextureCubeMapFactory->createMaterial("material", (ShaderDiffuseTextureCubeMap*)this->shader);
            assert(materialUPtr != nullptr);
            this->material = materialUPtr.get();
            scene->addToScene(std::move(materialUPtr));

            std::unique_ptr<TextureCubeMap> textureUPtr = diffuseTextureCubeMapFactory->createTexture("imageTexture");
            assert(textureUPtr != nullptr);
            std::unique_ptr<Image> frontImage(new Image);
            std::unique_ptr<Image> leftImage(new Image);
            std::unique_ptr<Image> backImage(new Image);
            std::unique_ptr<Image> rightImage(new Image);
            std::unique_ptr<Image> topImage(new Image);
            std::unique_ptr<Image> bottomImage(new Image);
            if (mapMode == CUBE_MAP_MODE_SIX_IMAGES){
                assert(textureImages.size() == 6);
                frontImage = std::move(textureImages[0]);
                leftImage = std::move(textureImages[1]);
                backImage = std::move(textureImages[2]);
                rightImage = std::move(textureImages[3]);
                topImage = std::move(textureImages[4]);
                bottomImage = std::move(textureImages[5]);
            }
            else if(mapMode == CUBE_MAP_MODE_SINGLE_IMAGE){
                assert(textureImages.size() == 1);
                assert(textureImages[0] != nullptr);
                std::unique_ptr<Image> originalImage = std::move(textureImages[0]);
                ImageModifier imageModifier(logger.get());
                imageModifier.convertSingleCubicImageIntoSix(originalImage.get(), frontImage.get(), leftImage.get(), backImage.get(), rightImage.get(), bottomImage.get(), topImage.get());//why I have to reverse top and bottom??
            }
            assert(rightImage != nullptr);
            textureUPtr->setTextureData(TEXTURE_CUBE_MAP_FACE_RIGHT, std::move(rightImage->data));
            textureUPtr->setHeight(TEXTURE_CUBE_MAP_FACE_RIGHT, rightImage->height);
            textureUPtr->setWidth(TEXTURE_CUBE_MAP_FACE_RIGHT, rightImage->width);
            textureUPtr->setTextureDataSize(TEXTURE_CUBE_MAP_FACE_RIGHT, rightImage->dataSize);

            assert(leftImage != nullptr);
            textureUPtr->setTextureData(TEXTURE_CUBE_MAP_FACE_LEFT, std::move(leftImage->data));
            textureUPtr->setHeight(TEXTURE_CUBE_MAP_FACE_LEFT, leftImage->height);
            textureUPtr->setWidth(TEXTURE_CUBE_MAP_FACE_LEFT, leftImage->width);
            textureUPtr->setTextureDataSize(TEXTURE_CUBE_MAP_FACE_LEFT, leftImage->dataSize);

            assert(topImage != nullptr);
            textureUPtr->setTextureData(TEXTURE_CUBE_MAP_FACE_TOP, std::move(topImage->data));
            textureUPtr->setHeight(TEXTURE_CUBE_MAP_FACE_TOP, topImage->height);
            textureUPtr->setWidth(TEXTURE_CUBE_MAP_FACE_TOP, topImage->width);
            textureUPtr->setTextureDataSize(TEXTURE_CUBE_MAP_FACE_TOP, topImage->dataSize);

            assert(bottomImage != nullptr);
            textureUPtr->setTextureData(TEXTURE_CUBE_MAP_FACE_BOTTOM, std::move(bottomImage->data));
            textureUPtr->setHeight(TEXTURE_CUBE_MAP_FACE_BOTTOM, bottomImage->height);
            textureUPtr->setWidth(TEXTURE_CUBE_MAP_FACE_BOTTOM, bottomImage->width);
            textureUPtr->setTextureDataSize(TEXTURE_CUBE_MAP_FACE_BOTTOM, bottomImage->dataSize);

            assert(backImage != nullptr);
            textureUPtr->setTextureData(TEXTURE_CUBE_MAP_FACE_BACK, std::move(backImage->data));
            textureUPtr->setHeight(TEXTURE_CUBE_MAP_FACE_BACK, backImage->height);
            textureUPtr->setWidth(TEXTURE_CUBE_MAP_FACE_BACK, backImage->width);
            textureUPtr->setTextureDataSize(TEXTURE_CUBE_MAP_FACE_BACK, backImage->dataSize);

            assert(frontImage != nullptr);
            textureUPtr->setTextureData(TEXTURE_CUBE_MAP_FACE_FRONT, std::move(frontImage->data));
            textureUPtr->setHeight(TEXTURE_CUBE_MAP_FACE_FRONT, frontImage->height);
            textureUPtr->setWidth(TEXTURE_CUBE_MAP_FACE_FRONT, frontImage->width);
            textureUPtr->setTextureDataSize(TEXTURE_CUBE_MAP_FACE_FRONT, frontImage->dataSize);

            this->imageTexture = textureUPtr.get();
            scene->addToScene(std::move(textureUPtr));
            ((MaterialDiffuseTextureCubeMap*)this->material)->setDiffuseTexture(this->imageTexture); 
            
            std::unique_ptr<TransformCamera> transformCameraUptr = transformCameraFactory->create(this->camera);
            this->camera->getComponentList().addComponent(std::move(transformCameraUptr));

            std::unique_ptr<TransformModel> transformSphereUptr = transformModelFactory->create(this->imageContainer);
            this->imageContainer->getComponentList().addComponent(std::move(transformSphereUptr));

            TransformCamera *transformCamera = (TransformCamera*)this->camera->getComponentList().getComponent("transform");
            transformCamera->setPosition(CL_Vec3(0.0f, 0.0f, 0.0f));
            transformCamera->setRotation(CL_Vec3(0.0f, 0.0f, 0.0f));

            TransformModel *transformSphere = (TransformModel*)this->imageContainer->getComponentList().getComponent("transform");
            transformSphere->setPosition(CL_Vec3(0.0f, 0.0f, 0.0f));

            this->imageContainer->createBiRelation(this->material);

            std::unique_ptr<ModelModifier> modelModifier(new ModelModifier);
            CubeBuilder cubeBuilder(modelModifier.get());
            cubeBuilder.buildInwardCube(this->imageContainer);
        }

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