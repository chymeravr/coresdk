#ifndef IMAGE360_IMAGE360_H
#define IMAGE360_IMAGE360_H

#include <coreEngine/IRenderer.h>
#include <coreEngine/events/EventKeyPressListener.h>
#include <coreEngine/events/EventPassiveMouseMotionListener.h>
#include <coreEngine/events/IEventQueue.h>
#include <coreEngine/factory/ICameraFactory.h>
#include <coreEngine/factory/IDiffuseTextureCubeMapFactory.h>
#include <coreEngine/factory/IDiffuseTextureFactory.h>
#include <coreEngine/factory/IModelFactory.h>
#include <coreEngine/factory/ISceneFactory.h>

#include <coreEngine/components/gazeDetector/GazeDetectorFactory.h>
#include <coreEngine/components/transformTree/ITransformTreeFactory.h>
#include <coreEngine/modifier/Image.h>
#include <coreEngine/ui/UIFactory.h>

#include <coreEngine/factory/IEventGazeListenerFactory.h>

namespace cl {
// todo : remove this
enum TEXTURE_MAP_MODE {
  CUBE_MAP_MODE_SIX_IMAGES,
  CUBE_MAP_MODE_SINGLE_IMAGE,
  EQUIRECTANGULAR_MAP_MODE
};
enum IMAGE_MODE { STEREO, MONO };

class Image360 : public EventKeyPressListener,
                 public EventPassiveMouseMotionListener {
 public:
  Image360(std::unique_ptr<IRenderer> renderer,
           std::unique_ptr<ISceneFactory> sceneFactory,
           std::unique_ptr<IModelFactory> modelFactory,
           std::unique_ptr<IDiffuseTextureFactory> diffuseTextureFactory,
           std::unique_ptr<IDiffuseTextureCubeMapFactory>
               diffuseTextureCubeMapFactory,
           std::unique_ptr<ITransformTreeFactory> transformTreeFactory,
           std::unique_ptr<ICameraFactory> cameraFactory,
           IEventQueue *eventQueue, ILoggerFactory *loggerFactory,
           std::unique_ptr<UIFactory> uiFactory,
           std::unique_ptr<GazeDetectorFactory> gazeDetectorFactory,
           std::unique_ptr<IEventGazeListenerFactory> gazeEventListenerFactory,
           std::string fontFolderPath) {
    assert(renderer != nullptr);
    assert(sceneFactory != nullptr);
    assert(modelFactory != nullptr);
    assert(diffuseTextureFactory != nullptr);
    assert(diffuseTextureCubeMapFactory != nullptr);
    assert(transformTreeFactory != nullptr);
    assert(eventQueue != nullptr);
    assert(cameraFactory != nullptr);
    assert(uiFactory != nullptr);
    assert(gazeDetectorFactory != nullptr);
    assert(gazeEventListenerFactory != nullptr);

    this->renderer = std::move(renderer);
    this->sceneFactory = std::move(sceneFactory);
    this->modelFactory = std::move(modelFactory);
    this->diffuseTextureFactory = std::move(diffuseTextureFactory);
    this->diffuseTextureCubeMapFactory =
        std::move(diffuseTextureCubeMapFactory);
    this->transformTreeFactory = std::move(transformTreeFactory);
    this->cameraFactory = std::move(cameraFactory);
    this->eventQueue = eventQueue;
    this->logger = loggerFactory->createLogger("Image360::");
    this->uiFactory = std::move(uiFactory);
    this->gazeDetectorFactory = std::move(gazeDetectorFactory);
    this->eventGazeListenerFactory = std::move(gazeEventListenerFactory);
    this->fontFolderPath = fontFolderPath;
  }

  // virtual ~Image360() = 0;
  // IApplication implementation
  void start();
  /**
  * @arg mode: One of the values of enum TEXTURE_MAP_MODE -
  * CUBE_MAP_MODE_SIX_IMAGES, CUBE_MAP_MODE_SINGLE_IMAGE,
  * EQUIRECTANGULAR_MAP_MODE
  * @arg textureImages: Images required to generate textures. In
  * case of CUBE_MAP_MODE_SIX_IMAGES order of images should be FRONT, LEFT,
  * BACK, RIGHT, TOP AND BOTTOM. In other cases just one image is required.
  */
  void initialize();
  void initMonoView();
  void initMonoEquirectangularView(std::unique_ptr<Image> textureImage);
  void initMonoCubeMapSingleTextureView(std::unique_ptr<Image> textureImage);
  void initMonoCubeMapSixTextureView(
      std::vector<std::unique_ptr<Image>> textureImage);

  void initStereoView();
  void initStereoEquirectangularView(std::unique_ptr<Image> textureImage);
  // note - implement stereo cubemaps if they catch on popularity

  void initUIButtons();
  void initCameraReticle();
  void initControllerReticle();
  void initFadeScreen();

  void initComplete();

  // TODO : review update functions
  void update();

  // draw is split into 4 steps - init, left eye, right eye, complete

  // draw init common stuff
  void drawInit();

  // void draw(EYE eye);

  // draw mono - no distinction between rendering for left and right eye
  void drawMono();
  // draw eye specific stuff - camera, models etc.
  void drawStereoRight();
  void drawStereoLeft();

  void drawComplete();
  void deinitialize();
  void stop();
  void pause();
  void resume();
  void onKeyPress(char key, int x, int y);
  void onPassiveMouseMotion(int x, int y);
  IRenderer *getRenderer();

  std::unique_ptr<EventGazeListener> actionButtonListener;
  std::unique_ptr<EventGazeListener> closeButtonListener;

  void setIsControllerPresent(bool isControllerPresent) {
    this->isControllerPresent = isControllerPresent;
  }

  void setCloseButtonText(std::string newCloseButtonText) {
    this->closeButtonText = newCloseButtonText;
  }

  void setActionButtonText(std::string newActionButtonText) {
    this->actionButtonText = newActionButtonText;
  }

  std::string getActionButtonText() { return this->actionButtonText; }

  void beginFade() { fadeStarted = true; }

  bool isFadeComplete() { return this->fadeComplete; }

 private:
  std::unique_ptr<Scene> scene;

  Camera *camera;

  // mono rendering
  Shader *shader;
  Material *material;
  Texture *imageTexture;
  Model *imageContainer;

  // stereo rendering
  Shader *stereoShader;
  Material *stereoMaterial;
  Texture *stereoImageTexture;
  Model *stereoImageContainer[2];  // 0->left, 1->right

  std::unique_ptr<IRenderer> renderer;
  IEventQueue *eventQueue;
  std::unique_ptr<ILogger> logger;

  std::unique_ptr<ISceneFactory> sceneFactory;
  std::unique_ptr<IModelFactory> modelFactory;
  std::unique_ptr<IDiffuseTextureFactory> diffuseTextureFactory;
  std::unique_ptr<IDiffuseTextureCubeMapFactory> diffuseTextureCubeMapFactory;
  std::unique_ptr<ITransformTreeFactory> transformTreeFactory;
  std::unique_ptr<ICameraFactory> cameraFactory;
  std::unique_ptr<UIFactory> uiFactory;
  std::unique_ptr<GazeDetectorFactory> gazeDetectorFactory;
  std::unique_ptr<IEventGazeListenerFactory> eventGazeListenerFactory;

  std::string fontFolderPath = "";

  std::string closeButtonText = "Close";
  std::string actionButtonText = "Notify Me";
  std::unique_ptr<PlanarBackground> actionButtonBackground;
  std::unique_ptr<PlanarBackground> closeButtonBackground;

  bool fadeStarted = false;
  bool fadeComplete = false;
  CL_GLfloat alphaFade = 0.0f;
  CL_GLfloat fadeSpeed = 0.01f;
  bool isControllerPresent = false;

  std::unique_ptr<FadeScreen> fadeScreen;
  std::unique_ptr<Reticle> reticle;
  std::unique_ptr<Reticle> reticleBase;
  std::unique_ptr<GazeDetectorContainer> gazeDetectorContainer;

  int lastPassiveMousePositionX = -1;
  int lastPassiveMousePositionY = -1;
  float passiveMouseMotionSensitivity = 0.35f;
  std::unique_ptr<FontStore> fontStore;
  TransformTree *gazeTransformTarget;

  void initCubeMapTexture(Image *rightImage, Image *leftImage, Image *topImage,
                          Image *bottomImage, Image *frontImage,
                          Image *backImage);
};
}

#endif  // IMAGE360_IMAGE360_H