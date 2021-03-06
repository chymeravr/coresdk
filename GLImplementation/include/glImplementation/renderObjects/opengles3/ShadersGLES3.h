#ifndef GLIMPLEMENTATION_SHADERSGLES3_H
#define GLIMPLEMENTATION_SHADERSGLES3_H

// we used global variable for representing shaders here within our own scope
// (cl)
// we do this to avoid handling file reading across platforms (win, android
// etc.)
namespace cl {
const std::string uniformColorGlVertexShader =
    "#version 300 es\n"
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

const std::string uniformColorGlFragmentShader =
    "#version 300 es \n"
    "// Ouput data\n"
    "precision mediump float; \n"
    "out vec4 color;\n"
    "// Values that stay constant for the whole mesh.\n"
    "uniform vec4 uniformColor;\n"
    "void main(){\n"
    "	// Output color = color of the texture at the specified UV\n"
    "	color = uniformColor;\n"
    "}";

const std::string diffuseTextureGlVertexShader =
    "#version 300 es\n"
    "// Input vertex data, different for all executions of this shader.\n"
    "layout(location = 0) in vec3 vertexPosition_modelspace;\n"
    "layout(location = 1) in vec2 vertexUV;\n"
    "// Output data ; will be interpolated for each fragment.\n"
    "out vec2 UV;\n"
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

const std::string diffuseTextureGlFragmentShader =
    "#version 300 es \n"
    "precision mediump float; \n"
    "// Interpolated values from the vertex shaders\n"
    "in vec2 UV;\n"
    "// Ouput data\n"
    "out vec4 color;\n"
    "// Values that stay constant for the whole mesh.\n"
    "uniform sampler2D diffuseTexture;\n"
    "void main(){\n"
    "// Output color = color of the texture at the specified UV\n"
    "color = texture(diffuseTexture, UV).rgba;\n"
    "}";

const std::string diffuseTextureAlphaGlFragmentShader =
    "#version 300 es \n"
    "precision mediump float; \n"
    "// Interpolated values from the vertex shaders\n"
    "in vec2 UV;\n"
    "// Ouput data\n"
    "out vec4 color;\n"
    "// Values that stay constant for the whole mesh.\n"
    "uniform sampler2D diffuseTexture;\n"
    "void main(){\n"
    "// Output color = color of the texture at the specified UV\n"
    "color = texture(diffuseTexture, UV).rgba;\n"
    "}";

const std::string diffuseTextureCubeMapGlVertexShader =
    "#version 300 es\n"
    "layout (location = 0) in vec3 vertex;\n"
    "uniform mat4 m;\n"
    "uniform mat4 v;\n"
    "uniform mat4 p;\n"
    "out vec3 texCoord;\n"
    "void main(){\n"
    "    gl_Position = p * v * m * vec4(vertex, 1.0); \n"
    "    texCoord = vertex;\n"
    "}\n";

const std::string diffuseTextureCubeMapGlFragmentShader =
    "#version 300 es \n"
    "precision mediump float; \n"
    "in vec3 texCoord;\n"
    "out vec4 color; \n"
    "uniform samplerCube diffuseTexture;\n"
    "void main(){\n"
    "   color = texture(diffuseTexture, texCoord);\n"
    "}";
}

#endif  // GLIMPLEMENTATION_SHADERSGLES3_H