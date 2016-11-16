//
// Created by chimeralabs on 10/24/2016.
//

#ifndef ANDROIDSDK_MATERIALUNIFORMCOLORFACTORY_H
#define ANDROIDSDK_MATERIALUNIFORMCOLORFACTORY_H

#include <coreEngine/factory/IObjectFactory.h>
#include <coreEngine/factory/defaultImpl/MaterialUniformColorFactoryParam.h>
#include <coreEngine/model/MaterialUniformColor.h>


namespace cl{
    class MaterialUniformColorFactory : public IObjectFactory{

    public:
        std::unique_ptr<Object> createObject(IObjectFactoryParam &param){
            MaterialUniformColorFactoryParam &materialParam = (MaterialUniformColorFactoryParam &) param;
            return std::unique_ptr<Object>(
                    new MaterialUniformColor(materialParam.getShader(), materialParam.getRenderer(),
                                 materialParam.getTag()));
        }

        void destroyObject(Object &object){

        }
    };
}

#endif //ANDROIDSDK_MATERIALUNIFORMCOLORFACTORY_H
