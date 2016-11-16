//
// Created by chimeralabs on 10/18/2016.
//

#ifndef ANDROIDSDK_SHADER_H
#define ANDROIDSDK_SHADER_H

#include <unordered_map>
#include <coreEngine/util/Logger.h>
#include <coreEngine/model/Object.h>
#include <coreEngine/model/IObjectRenderer.h>
#include <coreEngine/model/Camera.h>

namespace cl{
    class Shader : public Object{
    protected:
        IObjectRenderer *objectRendererPtr;
    public:
        Shader(IObjectRenderer &objectRenderer, std::string tag);

        IObjectRenderer &getRenderer(){
            return *objectRendererPtr;
        }
    };

}

#endif //ANDROIDSDK_SHADER_H
