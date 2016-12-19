#ifndef GLIMPLEMENTATION_SHADERPROGRAMUTIL
#define GLIMPLEMENTATION_SHADERPROGRAMUTIL

#include <vector>
#include <string>
#include <conf/GLTypes.h>
#include <coreEngine/util/ILogger.h>

namespace cl{
    class ShaderProgramUtil{
    public:
        static CL_GLuint createShaderProgram(const std::string &vertexShaderSrc, const std::string &fragmentShaderSrc, ILogger *logger);
    };
}

#endif //GLIMPLEMENTATION_SHADERPROGRAMUTIL