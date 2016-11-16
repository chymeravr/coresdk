//
// Created by chimeralabs on 10/21/2016.
//

#ifndef ANDROIDSDK_SCENEFACTORY_H
#define ANDROIDSDK_SCENEFACTORY_H

#include <coreEngine/factory/IObjectFactory.h>
#include <coreEngine/model/Scene.h>
#include "SceneFactoryParam.h"

namespace cl{
    class SceneFactory : public  IObjectFactory{

    public:
        std::unique_ptr<Object> createObject(IObjectFactoryParam &param) {
            SceneFactoryParam &sceneParam = (SceneFactoryParam&)param;
            return std::unique_ptr<Object>(new Scene(sceneParam.getRenderer(), param.getTag()));
        }

        void destroyObject(Object &object) {

        }
    };
}

#endif //ANDROIDSDK_SCENEFACTORY_H
