//
// Created by chimeralabs on 10/24/2016.
//

#ifndef ANDROIDSDK_MATERIALUNIFORMCOLORFACTORYPARAM_H
#define ANDROIDSDK_MATERIALUNIFORMCOLORFACTORYPARAM_H

#include <coreEngine/factory/defaultImpl/MaterialFactoryParam.h>
#include <coreEngine/model/Shader.h>
#include <coreEngine/model/IObjectRenderer.h>

namespace cl{
    class MaterialUniformColorFactoryParam : public MaterialFactoryParam{

    public:
        MaterialUniformColorFactoryParam(const std::string &tag, Shader &shader,
                                         IObjectRenderer &objectRenderer) : MaterialFactoryParam(
                tag, shader, objectRenderer) { }
    };
}

#endif //ANDROIDSDK_MATERIALUNIFORMCOLORFACTORYPARAM_H
