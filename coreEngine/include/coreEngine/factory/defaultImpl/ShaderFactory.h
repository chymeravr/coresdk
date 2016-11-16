//
// Created by chimeralabs on 10/21/2016.
//

#ifndef ANDROIDSDK_SHADERFACTORY_H
#define ANDROIDSDK_SHADERFACTORY_H

#include <coreEngine/factory/IObjectFactory.h>
#include <coreEngine/model/Shader.h>
#include "ShaderFactoryParam.h"

namespace cl{
    class ShaderFactory : public IObjectFactory{

    public:
        virtual std::unique_ptr<Object> createObject(IObjectFactoryParam &param) override {
            ShaderFactoryParam &shaderParam = (ShaderFactoryParam&) param;
            return std::unique_ptr<Object>(new Shader(shaderParam.getObjectRenderer(), shaderParam.getTag()));
        }

        virtual void destroyObject(Object &object) override {

        }
    };
}

#endif //ANDROIDSDK_SHADERFACTORY_H
