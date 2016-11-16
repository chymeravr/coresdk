//
// Created by chimeralabs on 10/26/2016.
//

#ifndef ANDROIDSDK_MODELSERVICE_H
#define ANDROIDSDK_MODELSERVICE_H

#include <coreEngine/service/IModelService.h>
#include <coreEngine/conf/Types.h>
#include <coreEngine/model/Model.h>
#include <coreEngine/service/IObjectService.h>

namespace cl{
    class ModelService : public IModelService{
    private:
        Logger *loggerPtr;
        std::unique_ptr<IObjectService> objectServicePtr;
    public:
        ModelService();
        std::pair<bool, Material&> getMaterialOfModel(Model &model);
        std::pair<bool, IObjectRenderer&> getRenderer(Model &model);
        void convertQuadIndicesToTriangleIndices(Model &model);
        void invertNormal(Model &model);
        void buildInwardCube(Model &model);
        void buildOutwardCube(Model &model);

    private:
        float linearInterp(float startY, float endY, float startX, float endX,
                           float targetX);
    };
}

#endif //ANDROIDSDK_MODELSERVICE_H
