//
// Created by chimeralabs on 10/24/2016.
//

#ifndef ANDROIDSDK_SHADERGLSERVICE_H
#define ANDROIDSDK_SHADERGLSERVICE_H

#include <conf/GLTypes.h>
#include <glImplementation/service/IShaderGLService.h>
#include <coreEngine/util/LoggerFactory.h>
#include <coreEngine/service/IObjectService.h>

namespace cl{
    class ShaderGLService : public IShaderGLService{
    private:
        Logger *loggerPtr = nullptr;
        std::unique_ptr<IObjectService> objectServicePtr;
    public:
        ShaderGLService();
        void createShaderProgram(ShaderGL &shaderGL);
        std::pair<bool, Camera&> getCamera(ShaderGL &shaderGL);
        void setCamera(ShaderGL &shaderGL, Camera &camera);
        void unsetCamera(ShaderGL &shaderGL, Camera &camera);
    };
}

#endif //ANDROIDSDK_SHADERGLSERVICE_H
