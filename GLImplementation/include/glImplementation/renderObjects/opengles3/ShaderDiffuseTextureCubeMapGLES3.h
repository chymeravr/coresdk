#ifndef GLIMPLEMENTATION_SHADERDIFFUSETEXTURECUBEMAPGLES3_H
#define GLIMPLEMENTATION_SHADERDIFFUSETEXTURECUBEMAPGLES3_H

#include <coreEngine/renderObjects/ShaderDiffuseTextureCubeMap.h>
#include <coreEngine/renderObjects/IRenderable.h>
#include <glImplementation/renderObjects/CameraGLContainer.h>

namespace cl
{
	class ShaderDiffuseTextureCubeMapGLES3 : public ShaderDiffuseTextureCubeMap, public IRenderable
	{
	public:
		ShaderDiffuseTextureCubeMapGLES3(const std::string &sceneId, ILoggerFactory *loggerFactory, Scene *scene);
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
		std::string vertexShaderSrc = "#version 300 es\n"
			"layout (location = 0) in vec3 vertex;\n"
			"uniform mat4 m;\n"
			"uniform mat4 v;\n"
			"uniform mat4 p;\n"
			"out vec3 texCoord;\n"
			"void main(){\n"
			"    gl_Position = p * v * m * vec4(vertex, 1.0); \n"
			"    texCoord = vertex;\n"
			"}\n";
		std::string fragmentShaderSrc = "#version 300 es \n"
			"precision mediump float; \n"
			"in vec3 texCoord;\n"
			"out vec4 color; \n"
			"uniform samplerCube diffuseTexture;\n"
			"void main(){\n"
			"   color = texture(diffuseTexture, texCoord);\n"
			"}";
	};
}

#endif //GLIMPLEMENTATION_SHADERDIFFUSETEXTURECUBEMAPGLES3_H