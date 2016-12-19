#ifndef GLIMPLEMENTATION_SHADERDIFFUSETEXTUREGL_H
#define GLIMPLEMENTATION_SHADERDIFFUSETEXTUREGL_H

#include <coreEngine/renderObjects/ShaderDiffuseTexture.h>
#include <coreEngine/renderObjects/IRenderable.h>
#include <glImplementation/renderObjects/CameraGLContainer.h>

namespace cl{
    class ShaderDiffuseTextureGL : public ShaderDiffuseTexture, public IRenderable{
        ShaderDiffuseTextureGL(const std::string &sceneId, ILoggerFactory *loggerFactory, Scene *scene);
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
            "\n"
            "// Input vertex data, different for all executions of this shader.\n"
            "layout(location = 0) in vec3 vertexPosition_modelspace;\n"
            "\n"
            "// Values that stay constant for the whole mesh.\n"
            "uniform mat4 m;\n"
            "uniform mat4 v;\n"
            "uniform mat4 p;\n"
            "\n"
            "void main(){\t\n"
            "\n"
            "\t// Output position of the vertex, in clip space : MVP * position\n"
            "\tgl_Position =  p * v * m * vec4(vertexPosition_modelspace,1);\n"
            "\n"
            "}\n";
        std::string fragmentShaderSrc = "#version 300 es\n"
            "\n"
            "// Ouput data\n"
            "out vec3 color;\n"
            "\n"
            "uniform vec3 uniformColor"
            "void main(){\n"
            "\n"
            "\tcolor = uniformColor;\n"
            "\n"
            "}";
    };
}

#endif //GLIMPLEMENTATION_SHADERDIFFUSETEXTUREGL_H