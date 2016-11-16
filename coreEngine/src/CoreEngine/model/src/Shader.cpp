//
// Created by chimeralabs on 11/8/2016.
//

#include <coreEngine/model/Shader.h>
#include <coreEngine/service/IServicePool.h>
#include <coreEngine/service/IObjectService.h>

namespace cl{

    Shader::Shader(IObjectRenderer &objectRenderer, std::string tag):Object("shader", tag){
        this->objectRendererPtr = &objectRenderer;
    }

}