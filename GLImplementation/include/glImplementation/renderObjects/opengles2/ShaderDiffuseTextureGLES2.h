#ifndef GLIMPLEMENTATION_SHADERDIFFUSETEXTUREGLES2_H
#define GLIMPLEMENTATION_SHADERDIFFUSETEXTUREGLES2_H

#include <coreEngine/renderObjects/ShaderDiffuseTexture.h>
#include <coreEngine/renderObjects/IRenderable.h>
#include <glImplementation/renderObjects/CameraGLContainer.h>

namespace cl
{
class ShaderDiffuseTextureGLES2 : public ShaderDiffuseTexture, public IRenderable
{
  public:
    ShaderDiffuseTextureGLES2(const std::string &sceneId, ILoggerFactory *loggerFactory, Scene *scene);
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
	"attribute vec3 vertexPosition_modelspace;\n"
	"attribute vec2 vertexUV;\n"
	"// Output data ; will be interpolated for each fragment.\n"
	"varying vec2 UV;\n"
	"// Values that stay constant for the whole mesh.\n"
	"uniform mat4 m;\n"
	"uniform mat4 v;\n"
	"uniform mat4 p;\n"
	"void main(){\n"
	"    // Output position of the vertex, in clip space : MVP * position \n"
	"    gl_Position = p * v * m * vec4(vertexPosition_modelspace, 1.0); \n"
	"    // UV of the vertex. No special space for this one. \n"
	"    UV = vertexUV;\n"
	"}\n";

    std::string fragmentShaderSrc =
	"precision mediump float; \n"
	"// Interpolated values from the vertex shaders\n"
	"varying vec2 UV;\n"
	"// Ouput data\n"
	"// varying vec4 color;\n"
	"// Values that stay constant for the whole mesh.\n"
	"uniform sampler2D diffuseTexture;\n"
	"void main(){\n"
	"// Output color = color of the texture at the specified UV\n"
	"vec4 color = texture2D(diffuseTexture, UV);\n"
	"gl_FragColor = color;\n"
	"}";
};
}

#endif //GLIMPLEMENTATION_SHADERDIFFUSETEXTUREGLES2_H