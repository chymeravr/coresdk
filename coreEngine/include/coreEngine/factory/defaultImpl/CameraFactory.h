//
// Created by chimeralabs on 10/25/2016.
//

#ifndef ANDROIDSDK_CAMERAFACTORY_H
#define ANDROIDSDK_CAMERAFACTORY_H

#include <coreEngine/factory/IObjectFactory.h>
#include <coreEngine/model/Camera.h>

namespace cl{
    class CameraFactory : public IObjectFactory{

    public:
        virtual std::unique_ptr<Object> createObject(IObjectFactoryParam &param) override {
            std::string tag = param.getTag();
            return std::unique_ptr<Object>(new Camera(tag));
        }

        virtual void destroyObject(Object &object) override {

        }
    };
}

#endif //ANDROIDSDK_CAMERAFACTORY_H
