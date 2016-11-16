//
// Created by chimeralabs on 10/27/2016.
//

#include <coreEngine/service/defaultImpl/ShaderService.h>
#include <coreEngine/service/IServicePool.h>

namespace cl{
    ShaderService::ShaderService() {
        objectServicePtr = IServicePool::getInstance().getIObjectService();
    }

    std::vector<Material *> ShaderService::getMaterials(Shader &shader) {
        std::pair<bool, std::vector<Object *>& > objectsPair = objectServicePtr->getLinkedObjectsByObjectType(shader, "material");
        if(!objectsPair.first){
            return std::vector<Material*>();
        }
        std::vector<Material *> materials;
        for(auto it=objectsPair.second.cbegin(); it!=objectsPair.second.cend(); it++){
            materials.push_back((Material*)(*it));
        }
        return materials;
    }

    std::pair<bool, IObjectRenderer&> ShaderService::getRenderer(Shader &shader) {
        return std::pair<bool, IObjectRenderer&>(true, shader.getRenderer());
    }

}