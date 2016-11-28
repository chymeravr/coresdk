//
// Created by chimeralabs on 10/26/2016.
//

#ifndef ANDROIDSDK_MODELSERVICE_H
#define ANDROIDSDK_MODELSERVICE_H

#include <coreEngine/service/IModelService.h>
#include <coreEngine/conf/Types.h>
#include <coreEngine/model/Model.h>
#include <coreEngine/service/IObjectService.h>
#include <coreEngine/util/ILogger.h>
#include <coreEngine/util/ILoggerFactory.h>

namespace cl{
    class ModelService : public IModelService{
    private:
        std::unique_ptr<ILogger> loggerPtr;
        std::unique_ptr<IObjectService> objectServicePtr;
    public:
        ModelService(ILoggerFactory *loggerFactoryPtr, std::unique_ptr<IObjectService> objectServicePtr);
        Material* getMaterialOfModel(Model *modelPtr);
        void invertNormal(Model *modelPtr);
        void buildInwardCube(Model *modelPtr);
        void buildOutwardCube(Model *modelPtr);

    private:
        void convertQuadIndicesToTriangleIndices(Model *modelPtr);
        float linearInterp(float startY, float endY, float startX, float endX,
                           float targetX);
    };
}

#endif //ANDROIDSDK_MODELSERVICE_H
