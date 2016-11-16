//
// Created by chimeralabs on 10/24/2016.
//

#ifndef ANDROIDSDK_SHADERUNIFORMCOLORFACTORYPARAM_H
#define ANDROIDSDK_SHADERUNIFORMCOLORFACTORYPARAM_H

#include <coreEngine/factory/defaultImpl/ShaderFactoryParam.h>

namespace cl{
    class ShaderUniformColorFactoryParam : public ShaderFactoryParam{

    public:
        ShaderUniformColorFactoryParam(const std::string &tag, IObjectRenderer &objectRenderer)
                : ShaderFactoryParam(tag, objectRenderer) { }
    };
}

#endif //ANDROIDSDK_SHADERUNIFORMCOLORFACTORYPARAM_H
