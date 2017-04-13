#ifndef GLIMPLEMENTATION_SHADERTEXTGLES2_H
#define GLIMPLEMENTATION_SHADERTEXTGLES2_H

#include <coreEngine/ui/ShaderText.h>
#include <coreEngine/renderObjects/IRenderable.h>
#include <glImplementation/renderObjects/CameraGLContainer.h>

namespace cl
{
class ShaderTextGLES2 : public ShaderText, IRenderable
{
  public:
    ShaderTextGLES2(const std::string &sceneId, ILoggerFactory *loggerFactory, Scene *scene);
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

    std::string vertexShaderSrc =
	"// Input vertex data, different for all executions of this shader.\n"
	"attribute vec3 vertex;\n"
	"attribute vec2 vertexUV;\n"
	"// Output data ; will be interpolated for each fragment.\n"
	"varying vec2 UV;\n"
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

    std::string fragmentShaderSrc =
	"precision mediump float; \n"
	"// Interpolated values from the vertex shaders\n"
	"varying vec2 UV;\n"
	"// Ouput data\n"
	"//out vec4 color;\n"
	"// Values that stay constant for the whole mesh.\n"
	"uniform vec4 textColor;\n"
	"uniform sampler2D textTexture;\n"
	"void main(){\n"
	"vec4 sampled = vec4(1.0, 1.0, 1.0, texture2D(textTexture, UV).r);\n"
	"vec4 color = textColor * sampled;\n"
	"gl_FragColor = color;\n"
	"}";
};
}

#endif //GLIMPLEMENTATION_SHADERTEXTGLES2_H