#ifndef GLIMPLEMENTATION_SHADERDIFFUSETEXTURECUBEMAPGL_H
#define GLIMPLEMENTATION_SHADERDIFFUSETEXTURECUBEMAPGL_H

#include <coreEngine/renderObjects/ShaderDiffuseTextureCubeMap.h>
#include <coreEngine/renderObjects/IRenderable.h>
#include <glImplementation/renderObjects/CameraGLContainer.h>

namespace cl
{
	class ShaderDiffuseTextureCubeMapGL : public ShaderDiffuseTextureCubeMap, public IRenderable
	{
	public:
		ShaderDiffuseTextureCubeMapGL(const std::string &sceneId, ILoggerFactory *loggerFactory, Scene *scene);
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
		std::string vertexShaderSrc = "#version 330\n"
			"layout (location = 0) in vec3 vertex;\n"
			"uniform mat4 m;\n"
			"uniform mat4 v;\n"
			"uniform mat4 p;\n"
			"out vec3 texCoord;\n"
			"void main(){\n"
			"    gl_Position = p * v * m * vec4(vertex, 1.0); \n"
			"    texCoord = vertex;\n"
			"}\n";
		std::string fragmentShaderSrc = "#version 330 \n"
			"in vec3 texCoord;\n"
			"out vec4 color; \n"
			"uniform samplerCube diffuseTexture;\n"
			"void main(){\n"
			"   color = texture(diffuseTexture, texCoord);\n"
			"}";
	};
}

#endif //GLIMPLEMENTATION_SHADERDIFFUSETEXTURECUBEMAPGL_H