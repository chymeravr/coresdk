//
// Created by chimeralabs on 10/27/2016.
//

#ifndef ANDROIDSDK_SHADERSERVICE_H
#define ANDROIDSDK_SHADERSERVICE_H

#include <coreEngine/service/IShaderService.h>
#include <coreEngine/service/IObjectService.h>

namespace cl{
    class ShaderService : public IShaderService{
    private:
        std::unique_ptr<IObjectService> objectServicePtr;
    public:
        ShaderService();
        std::vector<Material *> getMaterials(Shader &shader);
        std::pair<bool, IObjectRenderer&> getRenderer(Shader &shader);
    };
}

#endif //ANDROIDSDK_SHADERSERVICE_H
