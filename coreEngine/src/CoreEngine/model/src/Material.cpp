//
// Created by chimeralabs on 11/8/2016.
//

#include <coreEngine/model/Material.h>
#include <coreEngine/model/Shader.h>
#include <coreEngine/service/IObjectService.h>
#include <coreEngine/service/IServicePool.h>

namespace cl{

    Material::Material(Shader &shader, IObjectRenderer &objectRenderer, std::string tag) : Object("material", tag){
        auto objectServicePtr = IServicePool::getInstance().getIObjectService();
        objectServicePtr->linkObject((*this), shader);
        this->objectRendererPtr = &objectRenderer;
    }

}