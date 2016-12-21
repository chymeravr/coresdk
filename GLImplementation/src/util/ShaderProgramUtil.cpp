#include <glImplementation/util/ShaderProgramUtil.h>

namespace cl{
    CL_GLuint ShaderProgramUtil::createShaderProgram(const std::string &vertexShaderSrc, const std::string &fragmentShaderSrc, ILogger *logger){
        CL_GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
        CL_GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

        GLint result = GL_FALSE;
        GLint infoLogLength;

        //compile vertex shader
        const char *vertexShaderSourcePtr = vertexShaderSrc.c_str();
        glShaderSource(vertexShaderId, 1, &vertexShaderSourcePtr, NULL);
        glCompileShader(vertexShaderId);

        //read vertex shader status
        glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &result);
        if (!result){
            logger->log(LOG_ERROR, "Vertex shader compilation failed.");
        }
        glGetShaderiv(vertexShaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0){
            std::vector<char> vertexShaderErrorMessage(infoLogLength + 1);
            glGetShaderInfoLog(vertexShaderId, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
            logger->log(LOG_INFO, std::string(&vertexShaderErrorMessage[0]));
        }

        //compile fragmentshader
        const char *fragmentShaderSourcePtr = fragmentShaderSrc.c_str();
        glShaderSource(fragmentShaderId, 1, &fragmentShaderSourcePtr, NULL);
        glCompileShader(fragmentShaderId);

        //read fragmentshader status
        glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &result);
        if (!result){
            logger->log(LOG_ERROR, "Fragment shader compilation failed.");
        }
        glGetShaderiv(fragmentShaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0){
            std::vector<char> fragmentShaderErrorMessage(infoLogLength + 1);
            glGetShaderInfoLog(fragmentShaderId, infoLogLength, NULL, &fragmentShaderErrorMessage[0]);
            logger->log(LOG_INFO, std::string(&fragmentShaderErrorMessage[0]));
        }

        //link the program
        CL_GLuint  programId = glCreateProgram();
        glAttachShader(programId, vertexShaderId);
        glAttachShader(programId, fragmentShaderId);
        glLinkProgram(programId);

        // Check the program
        glGetProgramiv(programId, GL_LINK_STATUS, &result);
        if (!result){
            logger->log(LOG_ERROR, "Linking shader program failed.");
        }
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0){
            std::vector<char> programErrorMessage(infoLogLength + 1);
            glGetProgramInfoLog(programId, infoLogLength, NULL, &programErrorMessage[0]);
            logger->log(LOG_INFO, std::string(&programErrorMessage[0]));
        }

        glDeleteShader(vertexShaderId);
        glDeleteShader(fragmentShaderId);
        return programId;
    }
}