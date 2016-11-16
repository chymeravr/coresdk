//
// Created by chimeralabs on 10/21/2016.
//

#include <glImplementation/service/MaterialGLUniformColorRenderer.h>
#include <coreEngine/util/LoggerFactory.h>
#include <glImplementation/service/IServiceGLPool.h>
#include <glImplementation/model/MaterialGLUniformColor.h>
#include <glImplementation/model/ShaderGLUniformColor.h>
#include <coreEngine/model/MaterialUniformColor.h>

namespace cl{

    MaterialGLUniformColorRenderer::MaterialGLUniformColorRenderer() {
        loggerPtr = &LoggerFactory::getInstance().getLoggerInstance("app::glImplementation::MaterialGLUniformColorRenderer:");
        IServiceGLPool &serviceGLPool = IServiceGLPool::getInstance();
        materialService = serviceGLPool.getIMaterialService();
    }

    bool MaterialGLUniformColorRenderer::initialize(Object &object) {
        MaterialGLUniformColor &material = (MaterialGLUniformColor&) object;
        std::pair<bool, Shader &> shaderPair = materialService->getShader(material);
        if(shaderPair.first){
            ShaderGLUniformColor &shader = (ShaderGLUniformColor&)shaderPair.second;
            CL_GLuint programId = shader.getProgramId();
            CL_GLuint colorUniformObject = glGetUniformLocation(programId, "color");
            if(colorUniformObject == -1)
                return false;
            material.setColorUniformObject(colorUniformObject);
            loggerPtr->log(LOG_DEBUG, "Initialized.");
            return true;
        }else{
            loggerPtr->log(LOG_ERROR, "Shader couldn't be retrieved for material:" + material.getTag());
            return false;
        }

    }

    bool MaterialGLUniformColorRenderer::draw(Object &object) {
        MaterialGLUniformColor &material = (MaterialGLUniformColor&) object;
        CL_Vec3 color = material.getColor();
        CL_GLuint colorUniformObject = material.getColorUniformObject();
        glUniform3f(colorUniformObject, color[0], color[1], color[2]);
        loggerPtr->log(LOG_DEBUG, "Draw done.");
        return true;
    }

    bool MaterialGLUniformColorRenderer::deinitialize(Object &object) {
        return true;
    }

}