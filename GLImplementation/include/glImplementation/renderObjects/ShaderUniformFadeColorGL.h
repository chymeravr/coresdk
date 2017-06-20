/*
* We need a separate shader & material for fadeScreen because of issues with blending during with correct depth ordering
* TODO : document this somewhere
*/
#ifndef GLIMPLEMENTATION_SHADERUNIFORMFADECOLORGL_H
#define GLIMPLEMENTATION_SHADERUNIFORMFADECOLORGL_H

#include <coreEngine/renderObjects/ShaderUniformFadeColor.h>
#include <glImplementation/renderObjects/CameraGLContainer.h>
#include <glImplementation/renderObjects/Shaders.h>

namespace cl
{

class ShaderUniformFadeColorGL : public ShaderUniformFadeColor, public IRenderable
{
  public:
	ShaderUniformFadeColorGL(const std::string &sceneId, ILoggerFactory *loggerFactory, Scene *scene);
	IRenderable *getRenderable();
	/*
	* Here in initialize, set the program id to all those who need it (Model, Material etc.)
	*/
	bool initialize();
	void draw();
	void deinitialize();

  private:
	std::unique_ptr<ILogger> logger;
	CL_GLuint programId;
	std::unique_ptr<CameraGLContainer> cameraGLContainer;
	std::string vertexShaderSrc = uniformColorGlVertexShader; // ::uniformColorGlVertexShader;

	std::string fragmentShaderSrc = uniformColorGlFragmentShader;
};
}

#endif //GLIMPLEMENTATION_SHADERUNIFORMFADECOLORGL_H