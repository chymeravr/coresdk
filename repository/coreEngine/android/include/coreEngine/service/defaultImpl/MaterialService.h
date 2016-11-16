//
// Created by chimeralabs on 10/26/2016.
//

#ifndef ANDROIDSDK_MATERIALSERVICE_H
#define ANDROIDSDK_MATERIALSERVICE_H

#include<coreEngine/service/IMaterialService.h>
#include <coreEngine/util/LoggerFactory.h>
#include <coreEngine/service/IObjectService.h>

namespace cl{
    class MaterialService : public IMaterialService{
    private:
        std::unique_ptr<IObjectService> objectServicePtr;
        Logger *loggerPtr;
    public:
        MaterialService();
        void addMaterialToModel(Material &material, Model &model);
        std::vector<Model*> getModelsOfMaterial(Material& material);
        void removeMaterialFromModel(Material &material, Model &model);
        std::pair<bool, IObjectRenderer &> getRenderer(Material &material);
        std::pair<bool, Shader &> getShader(Material &material);
    };
}

#endif //ANDROIDSDK_MATERIALSERVICE_H
