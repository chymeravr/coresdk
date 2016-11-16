//
// Created by chimeralabs on 10/21/2016.
//

#include <coreEngine/factory/IObjectFactory.h>
#include <coreEngine/factory/IObjectFactoryParam.h>
#include <coreEngine/factory/defaultImpl/MaterialFactoryParam.h>
#include <coreEngine/model/Material.h>

#ifndef ANDROIDSDK_MATERIALFACTORY_H
#define ANDROIDSDK_MATERIALFACTORY_H

namespace cl{
    class MaterialFactory : public IObjectFactory{

    public:
        std::unique_ptr<Object> createObject(IObjectFactoryParam &param) {
            MaterialFactoryParam &materialParam = (MaterialFactoryParam &) param;
            return std::unique_ptr<Object>(
                    new Material(materialParam.getShader(), materialParam.getRenderer(),
                                 materialParam.getTag()));
        }

        virtual void destroyObject(Object &object) override {

        }
    };
}

#endif //ANDROIDSDK_MATERIALFACTORY_H
