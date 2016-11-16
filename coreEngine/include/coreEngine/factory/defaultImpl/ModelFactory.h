//
// Created by chimeralabs on 10/21/2016.
//

#ifndef ANDROIDSDK_MODELFACTORY_H
#define ANDROIDSDK_MODELFACTORY_H

#include <coreEngine/factory/IObjectFactory.h>
#include <coreEngine/model/Model.h>
#include <coreEngine/factory/defaultImpl/ModelFactoryParam.h>

namespace cl{
    class ModelFactory : public IObjectFactory{

    public:
        std::unique_ptr<Object> createObject(IObjectFactoryParam &param) {
            ModelFactoryParam &modelParams = (ModelFactoryParam&) param;
            std::string tag = modelParams.getTag();
            IObjectRenderer &renderer = modelParams.getRenderer();
            return std::unique_ptr<Object> (new Model(tag, renderer));
        }

        virtual void destroyObject(Object &object) override {

        }
    };
}

#endif //ANDROIDSDK_MODELFACTORY_H
