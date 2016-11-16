//
// Created by chimeralabs on 10/19/2016.
//

#ifndef ANDROIDSDK_MODELGLFACTORY_H
#define ANDROIDSDK_MODELGLFACTORY_H

#include <coreEngine/factory/IObjectFactory.h>
#include <glImplementation/model/ModelGL.h>
#include <coreEngine/factory/defaultImpl/ModelFactoryParam.h>

namespace cl{
    class ModelGLFactory : public IObjectFactory{

    public:
        std::unique_ptr<Object> createObject(IObjectFactoryParam &param){
            ModelFactoryParam &modelParams = (ModelFactoryParam&) param;
            std::string tag = modelParams.getTag();
            IObjectRenderer &renderer = modelParams.getRenderer();
            return std::unique_ptr<Object> (new ModelGL(tag, renderer));
        }

        void destroyObject(Object &object){

        }
    };
}

#endif //ANDROIDSDK_MODELGLFACTORY_H
