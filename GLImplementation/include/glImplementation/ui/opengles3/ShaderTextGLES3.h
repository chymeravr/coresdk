#ifndef GLIMPLEMENTATION_SHADERTEXTGLES3_H
#define GLIMPLEMENTATION_SHADERTEXTGLES3_H

#include <coreEngine/ui/ShaderText.h>
#include <coreEngine/renderObjects/IRenderable.h>
#include <glImplementation/renderObjects/CameraGLContainer.h>

namespace cl{
	class ShaderTextGLES3 : public ShaderText, IRenderable{
	public:
		ShaderTextGLES3(const std::string &sceneId, ILoggerFactory *loggerFactory, Scene *scene);
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
			"layout(location = 0) in vec3 vertex;\n"
			"layout(location = 1) in vec2 vertexUV;\n"
			"// Output data ; will be interpolated for each fragment.\n"
			"out vec2 UV;\n"
			"// Values that stay constant for the whole mesh.\n"
			"uniform mat4 m;\n"
			"uniform mat4 v;\n"
			"uniform mat4 p;\n"
			"void main(){\n"
			"    // Output position of the vertex, in clip space : MVP * position \n"
			"    gl_Position = p * v * m * vec4(vertex, 1); \n"
			"    // UV of the vertex. No special space for this one. \n"
			"    UV = vertexUV;\n"
			"}\n";

		std::string fragmentShaderSrc = "#version 300 es \n"
			"precision mediump float; \n"
			"// Interpolated values from the vertex shaders\n"
			"in vec2 UV;\n"
			"// Ouput data\n"
			"out vec4 color;\n"
			"// Values that stay constant for the whole mesh.\n"
			"uniform vec4 textColor;\n"
			"uniform sampler2D textTexture;\n"
			"void main(){\n"
			"vec4 sampled = vec4(1.0, 1.0, 1.0, texture(textTexture, UV).r);\n"
			"color = textColor * sampled;\n"
			"}";
	};
}

#endif //GLIMPLEMENTATION_SHADERTEXTGLES3_H