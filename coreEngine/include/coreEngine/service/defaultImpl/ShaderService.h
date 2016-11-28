//
// Created by chimeralabs on 10/27/2016.
//

#ifndef ANDROIDSDK_SHADERSERVICE_H
#define ANDROIDSDK_SHADERSERVICE_H

#include <coreEngine/service/IShaderService.h>
#include <coreEngine/service/IObjectService.h>
#include <coreEngine/util/ILogger.h>
#include <coreEngine/util/ILoggerFactory.h>

namespace cl{
    class ShaderService : public IShaderService{
    private:
        std::unique_ptr<ILogger> loggerPtr;
        std::unique_ptr<IObjectService> objectServicePtr;
    public:
        ShaderService(ILoggerFactory *loggerFactoryPtr, std::unique_ptr<IObjectService> objectServicePtr);
        std::vector<Material *> getMaterials(Shader *shaderPtr);
    };
}

#endif //ANDROIDSDK_SHADERSERVICE_H
