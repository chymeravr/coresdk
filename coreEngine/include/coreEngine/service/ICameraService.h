//
// Created by chimeralabs on 10/25/2016.
//

#ifndef ANDROIDSDK_ICAMERASERVICE_H
#define ANDROIDSDK_ICAMERASERVICE_H

#include <coreEngine/model/Camera.h>

namespace cl{
    class ICameraService{
    public:
        virtual void updateCameraViewMatrix(Camera *cameraPtr) = 0;
        virtual void updateCameraProjectionMatrix(Camera *cameraPtr) = 0;
    };
}

#endif //ANDROIDSDK_ICAMERASERVICE_H
