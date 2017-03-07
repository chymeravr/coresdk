#ifndef GLIMPLEMENTATION_SHADERUNIFORMCOLORGLES3_H
#define GLIMPLEMENTATION_SHADERUNIFORMCOLORGLES3_H

#include <coreEngine/renderObjects/ShaderUniformColor.h>
#include <glImplementation/renderObjects/CameraGLContainer.h>

namespace cl{
	class ShaderUniformColorGLES3 : public ShaderUniformColor, public IRenderable{
	public:
		ShaderUniformColorGLES3(const std::string &sceneId, ILoggerFactory *loggerFactory, Scene *scene);
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
			"// Input vertex data, different for all executions of this shader.\n"
			"layout(location = 0) in vec3 vertexPosition_modelspace;\n"
			"// Values that stay constant for the whole mesh.\n"
			"uniform mat4 m;\n"
			"uniform mat4 v;\n"
			"uniform mat4 p;\n"
			"void main(){\n"
			"    // Output position of the vertex, in clip space : MVP * position \n"
			"    gl_Position = p * v * m * vec4(vertexPosition_modelspace, 1); \n"
			"}\n";

		std::string fragmentShaderSrc = "#version 300 es \n"
			"// Ouput data\n"
			"precision mediump float; \n"
			"out vec4 color;\n"
			"// Values that stay constant for the whole mesh.\n"
			"uniform vec4 uniformColor;\n"
			"void main(){\n"
			"	// Output color = color of the texture at the specified UV\n"
			"	color = uniformColor;\n"
			"}";
	};
}

#endif //GLIMPLEMENTATION_SHADERUNIFORMCOLORGLES3_H