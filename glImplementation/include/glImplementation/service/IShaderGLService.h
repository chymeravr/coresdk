//
// Created by chimeralabs on 10/24/2016.
//

#ifndef ANDROIDSDK_ISHADERGLSERVICE_H
#define ANDROIDSDK_ISHADERGLSERVICE_H


#include <glImplementation/model/ShaderGL.h>

namespace cl{
    class IShaderGLService{
    public:
        virtual void createShaderProgram(ShaderGL &shaderGL) = 0;
        virtual std::pair<bool, Camera&> getCamera(ShaderGL &shaderGL) = 0;
        virtual void setCamera(ShaderGL &shaderGL, Camera &camera) = 0;
        virtual void unsetCamera(ShaderGL &shaderGL, Camera &camera) = 0;
    };
}

#endif //ANDROIDSDK_ISHADERGLSERVICE_H
