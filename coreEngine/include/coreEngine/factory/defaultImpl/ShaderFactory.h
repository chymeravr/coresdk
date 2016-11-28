//
// Created by chimeralabs on 10/21/2016.
//

#ifndef ANDROIDSDK_SHADERFACTORY_H
#define ANDROIDSDK_SHADERFACTORY_H

#include <coreEngine/factory/IShaderFactory.h>

namespace cl{
    class ShaderFactory : public IShaderFactory{

    public:
        virtual std::unique_ptr<Object> create(std::string tag, std::unique_ptr<IShaderRenderer> shaderRendererPtr) override{
            return std::unique_ptr<Object>(new Shader(std::move(shaderRendererPtr), tag));
        }

        virtual void destroy(Shader *shaderPtr) override {

        }
    };
}

#endif //ANDROIDSDK_SHADERFACTORY_H
