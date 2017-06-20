#ifndef IMAGE360_IMAGE360MONO_H
#define IMAGE360_IMAGE360MONO_H

#include <coreEngine/IRenderer.h>
#include <coreEngine/components/gazeDetector/GazeDetectorFactory.h>
#include <coreEngine/components/transformTree/ITransformTreeFactory.h>
#include <coreEngine/events/EventKeyPressListener.h>
#include <coreEngine/events/EventPassiveMouseMotionListener.h>
#include <coreEngine/events/IEventQueue.h>
#include <coreEngine/factory/ICameraFactory.h>
#include <coreEngine/factory/IDiffuseTextureCubeMapFactory.h>
#include <coreEngine/factory/IDiffuseTextureFactory.h>
#include <coreEngine/factory/IEventGazeListenerFactory.h>
#include <coreEngine/factory/IModelFactory.h>
#include <coreEngine/factory/ISceneFactory.h>
#include <coreEngine/modifier/Image.h>
#include <coreEngine/ui/UIFactory.h>
#include <image360/Image360.h>

namespace cl {
class Image360Mono : public Image360 {
 public:
  Image360Mono(
      std::unique_ptr<IRenderer> renderer,
      std::unique_ptr<ISceneFactory> sceneFactory,
      std::unique_ptr<IModelFactory> modelFactory,
      std::unique_ptr<IDiffuseTextureFactory> diffuseTextureFactory,
      std::unique_ptr<IDiffuseTextureCubeMapFactory>
          diffuseTextureCubeMapFactory,
      std::unique_ptr<ITransformTreeFactory> transformTreeFactory,
      std::unique_ptr<ICameraFactory> cameraFactory, IEventQueue *eventQueue,
      ILoggerFactory *loggerFactory, std::unique_ptr<UIFactory> uiFactory,
      std::unique_ptr<GazeDetectorFactory> gazeDetectorFactory,
      std::unique_ptr<IEventGazeListenerFactory> gazeEventListenerFactory,
      std::string fontFolderPath);

  ~Image360Mono(){};
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
  void initialize(TEXTURE_MAP_MODE mode,
                  std::vector<std::unique_ptr<Image>> &textureImages);
  void update();
  void drawInit();     // draw common stuff
  void draw(EYE eye);  // draw eye specific stuff - camera, models etc.
  void drawComplete();
  void deinitialize();  // todo - this has to be a virtual function as well
  void stop();
  void pause();
  void resume();
  void onKeyPress(char key, int x, int y);
  void onPassiveMouseMotion(int x, int y);
  IRenderer *getRenderer();

 protected:
  std::unique_ptr<Scene> scene;

  Camera *camera;
  Shader *shader;
  Material *material;
  Texture *imageTexture;
  Model *imageContainer;

  // TODO : move this to image360.h after testing
  std::unique_ptr<FadeScreen> fadeScreen;
  std::unique_ptr<Reticle> reticle;
  std::unique_ptr<Reticle> reticleBase;
  std::unique_ptr<GazeDetectorContainer> gazeDetectorContainer;

  int lastPassiveMousePositionX = -1;
  int lastPassiveMousePositionY = -1;
  float passiveMouseMotionSensitivity = 0.35f;
};
}

#endif  // IMAGE360_IMAGE360MONO_H