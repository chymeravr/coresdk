#ifndef IMAGE360_IMAGE360STEREO_H
#define IMAGE360_IMAGE360STEREO_H

#include <coreEngine/modifier/Image.h>
#include <coreEngine/IRenderer.h>
#include <coreEngine/factory/ISceneFactory.h>
#include <coreEngine/factory/IModelFactory.h>
#include <coreEngine/factory/ICameraFactory.h>
#include <coreEngine/events/EventKeyPressListener.h>
#include <coreEngine/events/EventPassiveMouseMotionListener.h>
#include <coreEngine/events/IEventQueue.h>
#include <coreEngine/factory/IDiffuseTextureFactory.h>
#include <coreEngine/factory/IDiffuseTextureCubeMapFactory.h>

#include <coreEngine/modifier/Image.h>
#include <coreEngine/ui/UIFactory.h>
#include <coreEngine/components/transformTree/ITransformTreeFactory.h>
#include <coreEngine/components/gazeDetector/GazeDetectorFactory.h>

#include <coreEngine/factory/IEventGazeListenerFactory.h>

namespace cl
{
class Image360Stereo : public Image360
{
  public:
    
	Image360Stereo(std::unique_ptr<IRenderer> renderer,
		std::unique_ptr<ISceneFactory> sceneFactory,
		std::unique_ptr<IModelFactory> modelFactory,
		std::unique_ptr<IDiffuseTextureFactory> diffuseTextureFactory,
		std::unique_ptr<IDiffuseTextureCubeMapFactory> diffuseTextureCubeMapFactory,
		std::unique_ptr<ITransformTreeFactory> transformTreeFactory,
		std::unique_ptr<ICameraFactory> cameraFactory,
		IEventQueue * eventQueue,
		ILoggerFactory * loggerFactory,
		std::unique_ptr<UIFactory> uiFactory,
		std::unique_ptr<GazeDetectorFactory> gazeDetectorFactory,
		std::unique_ptr<IEventGazeListenerFactory> eventGazeListenerFactory,
		std::string fontFolderPath);
	void initialize(TEXTURE_MAP_MODE mapMode, std::vector<std::unique_ptr<Image>> &textureImages);

	void drawInit();
	void draw(EYE eye);
	void drawComplete();
  private:
    void extractTexture(Image *image, EYE eye);
    Shader *stereoShader;
    Material *stereoMaterial;
    Texture *stereoImageTexture;
    Model *stereoImageContainer[2];
};
}

#endif // IMAGE360_IMAGE360STEREO_H