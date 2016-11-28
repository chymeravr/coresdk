//
// Created by chimeralabs on 10/25/2016.
//

#ifndef ANDROIDSDK_CAMERAFACTORY_H
#define ANDROIDSDK_CAMERAFACTORY_H

#include <coreEngine/factory/ICameraFactory.h>
#include <coreEngine/model/Camera.h>

namespace cl{
    class CameraFactory : public ICameraFactory{

    public:
        virtual std::unique_ptr<Object> create(std::string tag, std::unique_ptr<ICameraRenderer> cameraRendererPtr) override {
            return std::unique_ptr<Object>(new Camera(tag, std::move(cameraRendererPtr)));
        }

        virtual void destroy(Camera *cameraPtr) override {

        }
    };
}

#endif //ANDROIDSDK_CAMERAFACTORY_H
