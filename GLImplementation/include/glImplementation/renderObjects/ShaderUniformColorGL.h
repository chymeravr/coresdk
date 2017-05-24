#ifndef GLIMPLEMENTATION_SHADERUNIFORMCOLORGL_H
#define GLIMPLEMENTATION_SHADERUNIFORMCOLORGL_H

#include <coreEngine/renderObjects/ShaderUniformColor.h>
#include <glImplementation/renderObjects/CameraGLContainer.h>
#include <glImplementation/renderObjects/Shaders.h>

namespace cl{
	
	class ShaderUniformColorGL : public ShaderUniformColor, public IRenderable{
	public:
		ShaderUniformColorGL(const std::string &sceneId, ILoggerFactory *loggerFactory, Scene *scene);
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
		std::string vertexShaderSrc = uniformColorGlVertexShader;// ::uniformColorGlVertexShader;
		// std::string vertexShaderSrc = "#version 330 core\n"
		// 	"// Input vertex data, different for all executions of this shader.\n"
		// 	"layout(location = 0) in vec3 vertexPosition_modelspace;\n"
		// 	"// Values that stay constant for the whole mesh.\n"
		// 	"uniform mat4 m;\n"
		// 	"uniform mat4 v;\n"
		// 	"uniform mat4 p;\n"
		// 	"void main(){\n"
		// 	"    // Output position of the vertex, in clip space : MVP * position \n"
		// 	"    gl_Position = p * v * m * vec4(vertexPosition_modelspace, 1); \n"
		// 	"}\n";

		std::string fragmentShaderSrc = uniformColorGlFragmentShader;
		// std::string fragmentShaderSrc = "#version 330 core \n"
		// 	"// Ouput data\n"
		// 	"out vec4 color;\n"
		// 	"// Values that stay constant for the whole mesh.\n"
		// 	"uniform vec4 uniformColor;\n"
		// 	"void main(){\n"
		// 	"	// Output color = color of the texture at the specified UV\n"
		// 	"	color = uniformColor;\n"
		// 	"}";
	};
}

#endif //GLIMPLEMENTATION_SHADERUNIFORMCOLORGL_H