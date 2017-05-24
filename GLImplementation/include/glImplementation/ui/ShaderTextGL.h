#ifndef GLIMPLEMENTATION_SHADERTEXTGL_H
#define GLIMPLEMENTATION_SHADERTEXTGL_H

#include <coreEngine/ui/ShaderText.h>
#include <coreEngine/renderObjects/IRenderable.h>
#include <glImplementation/renderObjects/CameraGLContainer.h>
#include <glImplementation/ui/UiShaders.h>

namespace cl{
	class ShaderTextGL : public ShaderText, IRenderable{
	public:
		ShaderTextGL(const std::string &sceneId, ILoggerFactory *loggerFactory, Scene *scene);
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

		std::string vertexShaderSrc = textureTextGlVertexShader;

		std::string fragmentShaderSrc = textureTextGlFragmentShader;
	};
}

#endif //GLIMPLEMENTATION_SHADERTEXTGL_H