//
// Created by chimeralabs on 10/24/2016.
//

#ifndef ANDROIDSDK_MATERIALGLUNIFORMCOLORFACTORY_H
#define ANDROIDSDK_MATERIALGLUNIFORMCOLORFACTORY_H

#include <coreEngine/factory/IObjectFactory.h>
#include <glImplementation/model/MaterialGLUniformColor.h>
#include <coreEngine/factory/defaultImpl/MaterialUniformColorFactoryParam.h>

namespace cl{
    class MaterialGLUniformColorFactory : public IObjectFactory{

    public:
        std::unique_ptr<Object> createObject(IObjectFactoryParam &param){
            MaterialUniformColorFactoryParam &materialParam = (MaterialUniformColorFactoryParam &) param;
            return std::unique_ptr<Object>(
                    new MaterialGLUniformColor(materialParam.getShader(), materialParam.getRenderer(),
                                             materialParam.getTag()));
        }

        void destroyObject(Object &object){

        }
    };
}

#endif //ANDROIDSDK_MATERIALGLUNIFORMCOLORFACTORY_H
