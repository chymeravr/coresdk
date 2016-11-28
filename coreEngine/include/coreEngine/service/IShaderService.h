//
// Created by chimeralabs on 10/19/2016.
//

#ifndef ANDROIDSDK_ISHADERSERVICE_H
#define ANDROIDSDK_ISHADERSERVICE_H

#include <vector>
#include <coreEngine/model/Material.h>
#include <coreEngine/model/Shader.h>

namespace cl{

    class IShaderService{
    public:
        virtual std::vector<Material *> getMaterials(Shader *shaderPtr) = 0;
    };
}

#endif //ANDROIDSDK_ISHADERSERVICE_H
