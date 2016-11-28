//
// Created by chimeralabs on 10/26/2016.
//

#ifndef ANDROIDSDK_CAMERASERVICE_H
#define ANDROIDSDK_CAMERASERVICE_H

#include <coreEngine/service/ICameraService.h>

namespace cl{
    class CameraService : public ICameraService{
    public:
        void updateCameraViewMatrix(Camera *cameraPtr);
        void updateCameraProjectionMatrix(Camera *cameraPtr);
    };
}

#endif //ANDROIDSDK_CAMERASERVICE_H
