#include <assert.h>
#include <image360/Image360.h>
#include <coreEngine/modelBuilder/UVSphereBuilder.h>
#include <coreEngine/modifier/ModelModifier.h>
#include <coreEngine/modelBuilder/CubeBuilder.h>
#include <coreEngine/util/SimpleOBJLoader.h>
#include <coreEngine/modifier/ImageModifier.h>
#include <coreEngine/components/transformTree/TransformTreeFactory.h>

namespace cl{
	Image360::Image360(std::unique_ptr<IRenderer> renderer,
					   std::unique_ptr<ISceneFactory> sceneFactory,
					   std::unique_ptr<IModelFactory> modelFactory,
					   std::unique_ptr<IDiffuseTextureFactory> diffuseTextureFactory,
					   std::unique_ptr<IDiffuseTextureCubeMapFactory> diffuseTextureCubeMapFactory,
					   std::unique_ptr<ITransformCameraFactory> transformCameraFactory,
					   std::unique_ptr<ITransformModelFactory> transformModelFactory,
					   std::unique_ptr<ITransformTreeFactory> transformTreeFactory,
					   std::unique_ptr<ICameraFactory> cameraFactory,
					   IEventQueue *eventQueue, 
					   ILoggerFactory *loggerFactory,
					   std::unique_ptr<UIFactory> uiFactory,
					   std::unique_ptr<GazeDetectorFactory> gazeDetectorFactory){
		assert(renderer != nullptr);
		assert(sceneFactory != nullptr);
		assert(modelFactory != nullptr);
		assert(diffuseTextureFactory != nullptr);
		assert(diffuseTextureCubeMapFactory != nullptr);
		assert(transformCameraFactory != nullptr);
		assert(transformModelFactory != nullptr);
		assert(transformTreeFactory != nullptr);
		assert(eventQueue != nullptr);
		assert(cameraFactory != nullptr);
		assert(uiFactory != nullptr);
		assert(gazeDetectorFactory != nullptr);
		this->renderer = std::move(renderer);
		this->sceneFactory = std::move(sceneFactory);
		this->modelFactory = std::move(modelFactory);
		this->diffuseTextureFactory = std::move(diffuseTextureFactory);
		this->diffuseTextureCubeMapFactory = std::move(diffuseTextureCubeMapFactory);
		this->transformCameraFactory = std::move(transformCameraFactory);
		this->transformModelFactory = std::move(transformModelFactory);
		this->transformTreeFactory = std::move(transformTreeFactory);
		this->cameraFactory = std::move(cameraFactory);
		this->eventQueue = eventQueue;
		this->logger = loggerFactory->createLogger("image360::Image360: ");
		this->uiFactory = std::move(uiFactory);
		this->gazeDetectorFactory = std::move(gazeDetectorFactory);
	}

	//IApplication implementation
	void Image360::start(){
		this->renderer->start();
		this->logger->log(LOG_INFO, "Application started.");
	}
	void Image360::initialize(TEXTURE_MAP_MODE mapMode, std::vector<std::unique_ptr<Image>> &textureImages){
		std::unique_ptr<Camera> camera;
		scene = sceneFactory->create("testScene");
		assert(scene != nullptr);
		scene->setBackgroundColor(CL_Vec4(0.0f, 0.0f, 0.4f, 0.0f));
		scene->setDepthTest(true);
		scene->setBlending(true);
		camera = cameraFactory->create("camera", scene.get());
		assert(camera != nullptr);
		camera->setAspect(1.5f);
		camera->setFarPlane(100.0f);
		camera->setFov(1.5f);
		camera->setNearPlane(0.001f);
		this->camera = camera.get();
		scene->addToScene(std::move(camera));

		std::unique_ptr<Model> imageContainer = modelFactory->create("imageContainer");
		assert(imageContainer != nullptr);
		this->imageContainer = imageContainer.get();
		scene->addToScene(std::move(imageContainer));
		TransformTreeCamera *transformTreeCamera = nullptr;
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
			std::unique_ptr<TransformTreeCamera> transformTreeCameraUptr = transformTreeFactory->createTransformTreeCamera(this->camera);
			this->camera->getComponentList().addComponent(std::move(transformTreeCameraUptr));

			std::unique_ptr<TransformModel> transformSphereUptr = transformModelFactory->create(this->imageContainer);
			this->imageContainer->getComponentList().addComponent(std::move(transformSphereUptr));

			transformTreeCamera = (TransformTreeCamera*)this->camera->getComponentList().getComponent("transformTree");
			transformTreeCamera->setLocalPosition(CL_Vec3(0.0f, 0.0f, 0.0f));
			transformTreeCamera->setLocalRotation(CL_Vec3(0.0f, 0.0f, 0.0f));

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
			else if (mapMode == CUBE_MAP_MODE_SINGLE_IMAGE){
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

			std::unique_ptr<TransformTreeCamera> transformTreeCameraUptr = transformTreeFactory->createTransformTreeCamera(this->camera);
			this->camera->getComponentList().addComponent(std::move(transformTreeCameraUptr));

			std::unique_ptr<TransformModel> transformSphereUptr = transformModelFactory->create(this->imageContainer);
			this->imageContainer->getComponentList().addComponent(std::move(transformSphereUptr));

			transformTreeCamera = (TransformTreeCamera*)this->camera->getComponentList().getComponent("transformTree");
			transformTreeCamera->setLocalPosition(CL_Vec3(0.0f, 0.0f, 0.0f));
			transformTreeCamera->setLocalRotation(CL_Vec3(0.0f, 0.0f, 0.0f));

			TransformModel *transformSphere = (TransformModel*)this->imageContainer->getComponentList().getComponent("transform");
			transformSphere->setPosition(CL_Vec3(0.0f, 0.0f, 0.0f));

			this->imageContainer->createBiRelation(this->material);

			std::unique_ptr<ModelModifier> modelModifier(new ModelModifier);
			CubeBuilder cubeBuilder(modelModifier.get());
			cubeBuilder.buildInwardCube(this->imageContainer);

		}

		//Notify Me
		notifyMeBackground = uiFactory->createPlanarBackground("notifyMe", scene.get(), CL_Vec4(0.0, 0.0, 0.0, 0.7), CL_Vec3(-0.1, 0.0, -0.5), CL_Vec3(0.0, 0.0, 0.0), 0.1, 0.03);
		
		std::unique_ptr<FontStore> fontStore = uiFactory->createFontStore(scene.get(), "fonts/arial.ttf");
		
		TextStyle textStyle;
		textStyle.fontSize = 20;
		textStyle.scale = 0.0007f;
		textStyle.color = CL_Vec4(1.0, 1.0, 1.0, 1.0);
		
		std::unique_ptr<TextElement> notifyMeElement = uiFactory->createTextElement("notifyMeElement", fontStore.get(), &textStyle, "Notify Me", CL_Vec3(-0.03, -0.005, 0.001), CL_Vec3(0.0, 0.0, 0.0), scene.get());
		notifyMeBackground->addChild("child1", std::move(notifyMeElement));

		closeBackground = uiFactory->createPlanarBackground("closeMe", scene.get(), CL_Vec4(0.0, 0.0, 0.0, 0.7), CL_Vec3(0.1, 0.0, -0.5), CL_Vec3(0.0, 0.0, 0.0), 0.1, 0.03);
		std::unique_ptr<TextElement> closeElement = uiFactory->createTextElement("closeElement", fontStore.get(), &textStyle, "Close", CL_Vec3(-0.02, -0.005, 0.001), CL_Vec3(0.0, 0.0, 0.0), scene.get());
		closeBackground->addChild("child2", std::move(closeElement));
		
		reticle = uiFactory->createReticle("reticle", scene.get(), transformTreeCamera, CL_Vec4(0.0, 1.0, 0.0, 1.0));

		gazeDetectorContainer = gazeDetectorFactory->createGazeDetectorContainer();

		Model *notifyMeModel = (Model*) scene->getFromScene("notifyMe");
		TransformTreeModel *transformNotifyMe = (TransformTreeModel*)notifyMeModel->getComponentList().getComponent("transformTree");
		notifyMeListener = std::unique_ptr<NotifyMeListener>(new NotifyMeListener);
		std::unique_ptr<IComponent> gazeDetectorNotifyMe = gazeDetectorFactory->createGazeDetectorBox(std::string("notifyMe"), transformTreeCamera, transformNotifyMe, notifyMeListener.get(), gazeDetectorContainer.get(), CL_Vec3(0.0f, 0.0f, 0.0f), CL_Vec3(0.0f, 0.0f, -1.0f), 0.1f, 0.03f, 0.00001f);
		notifyMeModel->getComponentList().addComponent(std::move(gazeDetectorNotifyMe));
		
		Model *closeMeModel = (Model*)scene->getFromScene("closeMe");
		TransformTreeModel *transformCloseMe = (TransformTreeModel*)closeMeModel->getComponentList().getComponent("transformTree");
		closeMeListener = std::unique_ptr<CloseMeListener>(new CloseMeListener);
		std::unique_ptr<IComponent> gazeDetectorCloseMe = gazeDetectorFactory->createGazeDetectorBox(std::string("closeMe"), transformTreeCamera, transformCloseMe, closeMeListener.get(), gazeDetectorContainer.get(), CL_Vec3(0.0f, 0.0f, 0.0f), CL_Vec3(0.0f, 0.0f, -1.0f), 0.1f, 0.03f, 0.00001f);
		closeMeModel->getComponentList().addComponent(std::move(gazeDetectorCloseMe));

		renderer->initialize(scene.get());
	}
	void Image360::update(){
		renderer->update();
	}
	void Image360::draw(){
		while (!eventQueue->empty()){
			std::unique_ptr<IEvent> event = eventQueue->pop();
			event->callListener();
		}
		renderer->draw(scene.get());
	}
	void Image360::deinitialize(){
		renderer->deinitialize(scene.get());
	}
	void Image360::stop(){
		renderer->stop();
	}

	void Image360::onKeyPress(char key, int x, int y){
		//logger->log(LOG_DEBUG, "Key pressed:" + std::string(1, key));
	}

	void Image360::onPassiveMouseMotion(int x, int y){
		//logger->log(LOG_DEBUG, "Mouse move:" + std::to_string(x) + "," + std::to_string(y));
		if (lastPassiveMousePositionX != -1){
			float xoff = (x - lastPassiveMousePositionX)*passiveMouseMotionSensitivity;
			float yoff = (lastPassiveMousePositionY - y)*passiveMouseMotionSensitivity;

			TransformTreeCamera *transform = (TransformTreeCamera*)camera->getComponentList().getComponent("transformTree");
			CL_Vec3 rotation = transform->getLocalRotation();
			transform->setLocalRotation(CL_Vec3(rotation.x + yoff, rotation.y + xoff, rotation.z));
		}
		lastPassiveMousePositionX = x;
		lastPassiveMousePositionY = y;
	}

	IRenderer* Image360::getRenderer()
	{
		return this->renderer.get();
	}
}