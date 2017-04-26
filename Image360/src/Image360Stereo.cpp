#include <assert.h>
#include <image360/Image360Stereo.h>
#include <coreEngine/modelBuilder/UVSphereBuilder.h>
#include <coreEngine/modifier/ModelModifier.h>
#include <coreEngine/modelBuilder/CubeBuilder.h>
#include <coreEngine/util/SimpleOBJLoader.h>
#include <coreEngine/modifier/ImageModifier.h>
#include <coreEngine/components/transformTree/TransformTreeFactory.h>

namespace cl
{
Image360Stereo::Image360Stereo(std::unique_ptr<IRenderer> renderer,
                               std::unique_ptr<ISceneFactory> sceneFactory,
                               std::unique_ptr<IModelFactory> modelFactory,
                               std::unique_ptr<IDiffuseTextureFactory> diffuseTextureFactory,
                               std::unique_ptr<IDiffuseTextureCubeMapFactory> diffuseTextureCubeMapFactory,
                               std::unique_ptr<ITransformTreeFactory> transformTreeFactory,
                               std::unique_ptr<ICameraFactory> cameraFactory,
                               IEventQueue *eventQueue,
                               ILoggerFactory *loggerFactory,
                               std::unique_ptr<UIFactory> uiFactory,
                               std::unique_ptr<GazeDetectorFactory> gazeDetectorFactory,
                               std::unique_ptr<IEventGazeListenerFactory> eventGazeListenerFactory,
                               std::string fontFolderPath)
    : Image360(std::move(renderer), std::move(sceneFactory), std::move(modelFactory),
               std::move(diffuseTextureFactory), std::move(diffuseTextureCubeMapFactory),
               std::move(transformTreeFactory), std::move(cameraFactory),
               std::move(eventQueue), std::move(loggerFactory), std::move(uiFactory),
               std::move(gazeDetectorFactory), std::move(eventGazeListenerFactory),
               std::move(fontFolderPath))
{
    this->logger->setTag("Image360Stereo::");
}

Image360Stereo::~Image360Stereo()
{
}

void Image360Stereo::start()
{

    if (!renderer->start())
    {
        this->logger->log(LOG_INFO, "Application start failed");
        return;
    }
    this->logger->log(LOG_INFO, "Application started.");
}

void Image360Stereo::initialize(TEXTURE_MAP_MODE mapMode,
                                std::vector<std::unique_ptr<Image>> &textureImages)
{
    std::unique_ptr<Camera> camera;
    scene = sceneFactory->create("Image360:Stereo");
    assert(scene != nullptr);
    scene->setBackgroundColor(CL_Vec4(0.0f, 0.4f, 0.0f, 0.0f));
    scene->setDepthTest(true);
    scene->setBlending(true);
    camera = cameraFactory->create("camera", scene.get());
    assert(camera != nullptr);
    camera->setAspect(1.5f);
    camera->setFarPlane(1000.0f);
    camera->setFov(1.5f);
    camera->setNearPlane(1.0f);
    this->camera = camera.get();
    scene->addToScene(std::move(camera));

    std::unique_ptr<Model> imageContainerLeft = modelFactory->create("imageContainerLeft");
    assert(imageContainerLeft != nullptr);
    this->stereoImageContainer[0] = imageContainerLeft.get();
    scene->addToScene(std::move(imageContainerLeft));

    std::unique_ptr<Model> imageContainerRight = modelFactory->create("imageContainerRight");
    assert(imageContainerRight != nullptr);
    this->stereoImageContainer[1] = imageContainerRight.get();
    scene->addToScene(std::move(imageContainerRight));

    TransformTreeCamera *transformTreeCamera = nullptr;
    if (mapMode == EQUIRECTANGULAR_MAP_MODE)
    {
        std::unique_ptr<ShaderDiffuseTexture> shaderDiffuseTexture;
        std::unique_ptr<MaterialDiffuseTexture> materialDiffuseTexture;
        std::unique_ptr<Texture> imageTexture;

        shaderDiffuseTexture = diffuseTextureFactory->createShader("shader", scene.get());
        assert(shaderDiffuseTexture != nullptr);
        this->stereoShader = shaderDiffuseTexture.get();
        scene->addToScene(std::move(shaderDiffuseTexture));

        materialDiffuseTexture = diffuseTextureFactory->createMaterial("material",
                                                                       (ShaderDiffuseTexture *)this->stereoShader);
        assert(materialDiffuseTexture != nullptr);
        this->stereoMaterial = materialDiffuseTexture.get();
        scene->addToScene(std::move(materialDiffuseTexture));

        assert(textureImages.size() == 1);
        assert(textureImages[0] != nullptr);

        // todo - change texture mapping of models to do this
        imageTexture = diffuseTextureFactory->createTexture("imageTexture");
        assert(imageTexture != nullptr);

        std::unique_ptr<Image> image = std::move(textureImages[0]);
        imageTexture->setTextureData(std::move(image->data));
        imageTexture->setHeight(image->height);
        imageTexture->setWidth(image->width);
        imageTexture->setTextureDataSize(image->dataSize);
        this->stereoImageTexture = imageTexture.get();
        scene->addToScene(std::move(imageTexture));
        ((MaterialDiffuseTexture *)this->stereoMaterial)->setDiffuseTexture(this->stereoImageTexture);

        std::unique_ptr<TransformTreeCamera> transformTreeCameraUptr = transformTreeFactory->createTransformTreeCamera(this->camera);
        this->camera->getComponentList().addComponent(std::move(transformTreeCameraUptr));

        std::unique_ptr<TransformTreeModel> leftTransformSphereUptr = transformTreeFactory->createTransformTreeModel(this->stereoImageContainer[LEFT]);
        this->stereoImageContainer[LEFT]->getComponentList().addComponent(
            std::move(leftTransformSphereUptr));

        std::unique_ptr<TransformTreeModel> rightTransformSphereUptr = transformTreeFactory->createTransformTreeModel(this->stereoImageContainer[RIGHT]);
        this->stereoImageContainer[RIGHT]->getComponentList().addComponent(
            std::move(rightTransformSphereUptr));

        transformTreeCamera = (TransformTreeCamera *)this->camera->getComponentList().getComponent("transformTree");
        transformTreeCamera->setLocalPosition(CL_Vec3(0.0f, 0.0f, 0.0f));
        transformTreeCamera->setLocalRotation(CL_Vec3(0.0f, 0.0f, 0.0f));

        TransformTreeModel *transformSphereLeft = (TransformTreeModel *)this->stereoImageContainer[LEFT]->getComponentList().getComponent("transformTree");
        transformSphereLeft->setLocalPosition(CL_Vec3(0.0f, 0.0f, 0.0f));
        transformSphereLeft->setLocalScale(CL_Vec3(100.0f, 100.0f, 100.0f));

        this->stereoImageContainer[LEFT]->createBiRelation(this->stereoMaterial);

        TransformTreeModel *transformSphereRight = (TransformTreeModel *)this->stereoImageContainer[RIGHT]->getComponentList().getComponent("transformTree");
        transformSphereRight->setLocalPosition(CL_Vec3(0.0f, 0.0f, 0.0f));
        transformSphereRight->setLocalScale(CL_Vec3(100.0f, 100.0f, 100.0f));

        this->stereoImageContainer[RIGHT]->createBiRelation(this->stereoMaterial);

        std::unique_ptr<ModelModifier> modelModifierLeft(new ModelModifier);
        UVSphereBuilder uvSphereBuilderLeft(modelModifierLeft.get());
        uvSphereBuilderLeft.setVMax(0.5f);
        uvSphereBuilderLeft.buildUnitSphere(this->stereoImageContainer[LEFT], 5);

        std::unique_ptr<ModelModifier> modelModifierRight(new ModelModifier);
        UVSphereBuilder uvSphereBuilderRight(modelModifierRight.get());
        uvSphereBuilderRight.setVMin(0.5f);
        uvSphereBuilderRight.buildUnitSphere(this->stereoImageContainer[RIGHT], 5);
    }
    else
    {
        //todo - CubeMap format for stereo images
    }

    //Notify Me
    auto vec3_one = CL_Vec3(-5.1, 0.0, -15.5);
    auto vec3_two = CL_Vec3(0.0, 0.0, 0.0);
    notifyMeBackground = uiFactory->createPlanarBackground("notifyMe", scene.get(), CL_Vec4(0.0, 0.0, 0.0, 0.7), vec3_one, vec3_two, 3.0, 1.0);

    std::unique_ptr<FontStore> fontStore = uiFactory->createFontStore(scene.get(), this->fontFolderPath.c_str());

    TextStyle textStyle;
    textStyle.fontSize = 20;
    textStyle.scale = 0.025f;
    textStyle.color = CL_Vec4(1.0, 1.0, 1.0, 1.0);

    vec3_one = CL_Vec3(-1.0, -0.2, 0.001);
    vec3_two = CL_Vec3(0.0, 0.0, 0.0);
    std::unique_ptr<TextElement> notifyMeElement = uiFactory->createTextElement("notifyMeElement", fontStore.get(), &textStyle, "Notify Me", vec3_one,
                                                                                vec3_two, scene.get());
    notifyMeBackground->addChild("child1", std::move(notifyMeElement));

    vec3_one = CL_Vec3(5.1, 0.0, -15.5);
    vec3_two = CL_Vec3(0.0, 0.0, 0.0);
    closeBackground = uiFactory->createPlanarBackground("closeMe", scene.get(), CL_Vec4(0.0, 0.0, 0.0, 0.7), vec3_one,
                                                        vec3_two, 3.0, 1.0);

    vec3_one = CL_Vec3(-1.0, -0.2, 1.0);
    vec3_two = CL_Vec3(0.0, 0.0, 0.0);
    std::unique_ptr<TextElement> closeElement = uiFactory->createTextElement("closeElement", fontStore.get(), &textStyle, "Close",
                                                                             vec3_one, vec3_two, scene.get());
    closeBackground->addChild("child2", std::move(closeElement));

    // auto vec3_zero = CL_Vec4(0.0, 1.0, 0.0, 1.0);
    // reticle = uiFactory->createReticle("reticle", scene.get(), transformTreeCamera, vec3_zero);

    gazeDetectorContainer = gazeDetectorFactory->createGazeDetectorContainer();

    Model *notifyMeModel = (Model *)scene->getFromScene("notifyMe");
    TransformTreeModel *transformNotifyMe = (TransformTreeModel *)notifyMeModel->getComponentList().getComponent(
        "transformTree");
    notifyMeListener = eventGazeListenerFactory->createNotifyMeListener();

    Model *closeMeModel = (Model *)scene->getFromScene("closeMe");
    TransformTreeModel *transformCloseMe = (TransformTreeModel *)closeMeModel->getComponentList().getComponent("transformTree");
    closeMeListener = eventGazeListenerFactory->createCloseMeListener();

    TransformTree *gazeTransformTarget = nullptr;
    auto vec3_zero = CL_Vec4(0.0, 1.0, 0.0, 1.0);
    if (isControllerPresent)
    {
        reticleBase = uiFactory->createReticle("reticleBase", scene.get(), nullptr, vec3_zero);
        TransformTreeModel *transform = reticleBase->getTransformTreeModel();
        reticle = uiFactory->createReticle("reticle", scene.get(), transform, vec3_zero);
        gazeTransformTarget = transform;
    }
    else
    {
        reticle = uiFactory->createReticle("reticle", scene.get(), transformTreeCamera, vec3_zero);
        gazeTransformTarget = transformTreeCamera;
    }

    auto boxMessage = std::string("notifyMe");
    vec3_one = CL_Vec3(0.0f, 0.0f, 0.0f);
    vec3_two = CL_Vec3(0.0f, 0.0f, -1.0f);
    std::unique_ptr<IComponent> gazeDetectorNotifyMe = gazeDetectorFactory->createGazeDetectorBox(boxMessage, gazeTransformTarget,
                                                                                                  transformNotifyMe, notifyMeListener.get(),
                                                                                                  gazeDetectorContainer.get(), vec3_one,
                                                                                                  vec3_two, 3.0f, 1.0f, 0.00001f);
    notifyMeModel->getComponentList().addComponent(std::move(gazeDetectorNotifyMe));

    auto boxMessage2 = std::string("closeMe");
    vec3_one = CL_Vec3(0.0f, 0.0f, 0.0f);
    vec3_two = CL_Vec3(0.0f, 0.0f, -1.0f);
    std::unique_ptr<IComponent> gazeDetectorCloseMe = gazeDetectorFactory->createGazeDetectorBox(boxMessage2, gazeTransformTarget,
                                                                                                 transformCloseMe, closeMeListener.get(),
                                                                                                 gazeDetectorContainer.get(), vec3_one,
                                                                                                 vec3_two, 3.0f, 1.0f, 0.00001f);
    closeMeModel->getComponentList().addComponent(std::move(gazeDetectorCloseMe));

    renderer->initialize(scene.get());
    logger->log(LOG_DEBUG, "Initialization Completed!!");
}

void Image360Stereo::deinitialize()
{
    renderer->deinitialize(scene.get());
}

void Image360Stereo::drawInit()
{
    while (!eventQueue->empty())
    {
        std::unique_ptr<IEvent> event = eventQueue->pop();
        event->callListener();
    }
    // Render objects common to both eyes
    renderer->drawInit(scene.get());
}

void Image360Stereo::draw(EYE eye)
{
    auto rightContainer = (Model *)this->scene->getFromScene("imageContainerRight");
    auto leftContainer = (Model *)this->scene->getFromScene("imageContainerLeft");

    if (eye == LEFT)
    {
        leftContainer->setIsVisible(true);
        rightContainer->setIsVisible(false);
    }
    else if (eye == RIGHT)
    {
        leftContainer->setIsVisible(false);
        rightContainer->setIsVisible(true);
    }
    else
    {
        return;
    }
    renderer->draw(scene.get(), eye);
}

void Image360Stereo::drawComplete()
{
    renderer->drawComplete();
}

void Image360Stereo::update()
{
    renderer->update();
}

void Image360Stereo::pause()
{
    renderer->pause();
}

void Image360Stereo::resume()
{
    renderer->resume();
}

void Image360Stereo::stop()
{
    renderer->stop();
}

void Image360Stereo::onKeyPress(char key, int x, int y)
{
    logger->log(LOG_DEBUG, "Key pressed:" + std::string(1, key));
    TransformTreeCamera *transform = (TransformTreeCamera *)camera->getComponentList().getComponent("transformTree");
    CL_Vec3 pos = transform->getLocalPosition();

    if (key == 'W')
    {
        pos[2] -= 0.6f;
    }
    else if (key == 'S')
    {
        pos[2] += 0.6f;
    }
    else if (key == 'A')
    {
        pos[0] -= 0.6f;
    }
    else if (key == 'D')
    {
        pos[0] += 0.6f;
    }
    else
    {
        return;
    }
    transform->setLocalPosition(pos);
}

void Image360Stereo::onPassiveMouseMotion(int x, int y)
{
    //logger->log(LOG_DEBUG, "Mouse move:" + std::to_string(x) + "," + std::to_string(y));
    if (lastPassiveMousePositionX != -1)
    {
        float xoff = (x - lastPassiveMousePositionX) * passiveMouseMotionSensitivity;
        float yoff = (y - lastPassiveMousePositionY) * passiveMouseMotionSensitivity;

        TransformTreeCamera *transform = (TransformTreeCamera *)camera->getComponentList().getComponent("transformTree");
        CL_Vec3 rotation = transform->getLocalRotation();
        transform->setLocalRotation(CL_Vec3(rotation.x - yoff, rotation.y - xoff, rotation.z));
    }
    lastPassiveMousePositionX = x;
    lastPassiveMousePositionY = y;
}

IRenderer *Image360Stereo::getRenderer()
{
    return this->renderer.get();
}
}