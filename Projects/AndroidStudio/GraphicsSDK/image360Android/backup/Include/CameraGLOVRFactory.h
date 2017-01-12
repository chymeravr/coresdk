//
// Created by robin_chimera on 1/9/2017.
//

#ifndef GRAPHICSSDK_CAMERAGLOVRFACTORY_H
#define GRAPHICSSDK_CAMERAGLOVRFACTORY_H


#include <coreEngine/factory/ICameraFactory.h>
#include "CameraGLOVR.h"

namespace cl{
    class CameraGLOVRFactory : public ICameraFactory{
    public:
        CameraGLOVRFactory(ILoggerFactory *loggerFactory){
            this->loggerFactory = loggerFactory;
        }
        std::unique_ptr<Camera> create(const std::string &sceneId, Scene *scene){
            return std::unique_ptr<Camera>(new CameraGLOVR(sceneId, loggerFactory, scene));
        }
    private:
        ILoggerFactory *loggerFactory;
    };
}

#endif //GRAPHICSSDK_CAMERAGLOVRFACTORY_H
