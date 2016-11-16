//
// Created by chimeralabs on 10/19/2016.
//

#ifndef ANDROIDSDK_ISHADERSERVICE_H
#define ANDROIDSDK_ISHADERSERVICE_H

#include <vector>
#include <coreEngine/model/Material.h>
#include <coreEngine/model/Shader.h>

namespace cl{
    class Material;
    class Shader;
    class Camera;

    class IShaderService{
    public:
        virtual std::vector<Material *> getMaterials(Shader &shader) = 0;
        virtual std::pair<bool, IObjectRenderer&> getRenderer(Shader &shader) = 0;
    };
}

#endif //ANDROIDSDK_ISHADERSERVICE_H
