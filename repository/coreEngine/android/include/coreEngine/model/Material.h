//
// Created by chimeralabs on 10/18/2016.
//

#ifndef ANDROIDSDK_MATERIAL_H
#define ANDROIDSDK_MATERIAL_H

#include <memory>

#include <coreEngine/model/Object.h>
#include <coreEngine/util/Logger.h>
#include <coreEngine/model/IObjectRenderer.h>

namespace cl{
    class Shader;
    class Material : public Object{
    protected:
        IObjectRenderer *objectRendererPtr;
    public:
        Material(Shader &shader, IObjectRenderer &objectRenderer, std::string tag);
        IObjectRenderer& getObjectRenderer(){
            return *objectRendererPtr;
        }
    };
}

#endif //ANDROIDSDK_MATERIAL_H
