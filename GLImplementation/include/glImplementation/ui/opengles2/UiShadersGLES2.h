#ifndef GLIMPLEMENTATION_UISHADERSGLES2_H
#define GLIMPLEMENTATION_UISHADERSGLES2_H

// we used global variable for representing shaders here within our own scope (cl)
// we do this to avoid handling file reading across platforms (win, android etc.)

/*
	GLES2 has different semantics for shader function texture and texture2D and hence
	the segregation of opengles2 and opengles3 
*/

namespace cl{
	const std::string textureTextGlVertexShader = "// Input vertex data, different for all executions of this shader.\n"
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

	const std::string textureTextGlFragmentShader = "precision mediump float; \n"
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
            
}

#endif //GLIMPLEMENTATION_UISHADERSGLES2_H