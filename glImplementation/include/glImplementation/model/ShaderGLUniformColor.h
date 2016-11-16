//
// Created by chimeralabs on 10/19/2016.
//

#ifndef ANDROIDSDK_SHADERGLUNIFORMCOLOR_H
#define ANDROIDSDK_SHADERGLUNIFORMCOLOR_H

#include <glImplementation/model/ShaderGL.h>
#include <conf/GLTypes.h>

namespace cl{
    class ShaderGLUniformColor : public ShaderGL{
    private:
    public:
        ShaderGLUniformColor(IObjectRenderer &objectRenderer, const std::string &tag)
                : ShaderGL(objectRenderer, tag){
            vertexShaderCode = "#version 300 es\n"
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
            fragmentShaderCode = "#version 300 es\n"
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
        }

        void createMVPUniformObjects() {
            modelUniformObject = glGetUniformLocation(getProgramId(), "m");
            viewUniformObject = glGetUniformLocation(getProgramId(), "v");
            projectionUniformObject = glGetUniformLocation(getProgramId(), "p");
        }

    };
}

#endif //ANDROIDSDK_SHADERGLUNIFORMCOLOR_H
