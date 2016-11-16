//
// Created by chimeralabs on 10/24/2016.
//

#ifndef ANDROIDSDK_SHADERGLUNIFORMCOLORFACTORY_H
#define ANDROIDSDK_SHADERGLUNIFORMCOLORFACTORY_H

#include <coreEngine/factory/IObjectFactory.h>
#include <glImplementation/model/ShaderGLUniformColor.h>
#include <coreEngine/factory/defaultImpl/ShaderUniformColorFactoryParam.h>

namespace cl{
    class ShaderGLUniformColorFactory : public IObjectFactory{

    public:
        virtual std::unique_ptr<Object> createObject(IObjectFactoryParam &param) override {
            ShaderUniformColorFactoryParam &shaderParam = (ShaderUniformColorFactoryParam&) param;
            return std::unique_ptr<Object>(
                    new ShaderGLUniformColor(shaderParam.getObjectRenderer(), shaderParam.getTag()));
        }

        virtual void destroyObject(Object &object) override {

        }
    };
}

#endif //ANDROIDSDK_SHADERGLUNIFORMCOLORFACTORY_H
