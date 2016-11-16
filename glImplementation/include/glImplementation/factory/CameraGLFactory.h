//
// Created by chimeralabs on 10/25/2016.
//

#ifndef ANDROIDSDK_CAMERAGLFACTORY_H
#define ANDROIDSDK_CAMERAGLFACTORY_H

#include <coreEngine/factory/IObjectFactory.h>
#include <glImplementation/model/CameraGL.h>

namespace cl{
    class CameraGLFactory : public IObjectFactory{

    public:
        virtual std::unique_ptr<Object> createObject(IObjectFactoryParam &param) override {
            return std::unique_ptr<Object>(new CameraGL(param.getTag()));
        }

        virtual void destroyObject(Object &object) override {

        }
    };
}

#endif //ANDROIDSDK_CAMERAGLFACTORY_H
