//
// Created by chimeralabs on 10/26/2016.
//

#ifndef ANDROIDSDK_MATERIALSERVICE_H
#define ANDROIDSDK_MATERIALSERVICE_H

#include<coreEngine/service/IMaterialService.h>
#include <coreEngine/util/ILoggerFactory.h>
#include <coreEngine/service/IObjectService.h>

namespace cl{
    class MaterialService : public IMaterialService{
    private:
        std::unique_ptr<IObjectService> objectServicePtr;
        std::unique_ptr<ILogger> loggerPtr;
    public:
        MaterialService(ILoggerFactory *loggerFactoryPtr, std::unique_ptr<IObjectService> objectServicePtr);
        void addMaterialToModel(Material *materialPtr, Model *modelPtr);
        std::vector<Model*> getModelsOfMaterial(Material* materialPtr);
        void removeMaterialFromModel(Material *materialPtr, Model *modelPtr);
        Shader* getShader(Material *materialPtr);
    };
}

#endif //ANDROIDSDK_MATERIALSERVICE_H
