//
// Created by chimeralabs on 10/18/2016.
//

#ifndef ANDROIDSDK_IMATERIALSERVICE_H
#define ANDROIDSDK_IMATERIALSERVICE_H

#include <vector>
#include <coreEngine/model/Material.h>
#include <coreEngine/model/Model.h>
#include <coreEngine/model/Shader.h>

namespace cl{
    class Material;
    class Shader;

    class IMaterialService{
    public:
        virtual void addMaterialToModel(Material *materialPtr, Model *modelPtr) = 0;
        virtual std::vector<Model*> getModelsOfMaterial(Material* materialPtr) = 0;
        virtual void removeMaterialFromModel(Material *materialPtr, Model *modelPtr) = 0;
        virtual Shader* getShader(Material *materialPtr) = 0;
    };
}

#endif //ANDROIDSDK_IMATERIALSERVICE_H
