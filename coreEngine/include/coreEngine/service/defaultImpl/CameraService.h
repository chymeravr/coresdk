//
// Created by chimeralabs on 10/26/2016.
//

#ifndef ANDROIDSDK_CAMERASERVICE_H
#define ANDROIDSDK_CAMERASERVICE_H

#include <coreEngine/service/ICameraService.h>

namespace cl{
    class CameraService : public ICameraService{
    public:
        void updateCamera(Camera &camera);
    };
}

#endif //ANDROIDSDK_CAMERASERVICE_H
