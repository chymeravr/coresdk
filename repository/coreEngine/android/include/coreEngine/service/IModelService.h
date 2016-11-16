//
// Created by chimeralabs on 10/18/2016.
//

#ifndef ANDROIDSDK_IMODELSERVICE_H
#define ANDROIDSDK_IMODELSERVICE_H

#include <coreEngine/model/Material.h>

namespace cl{
    class Model;

    class IModelService{
    public:
        virtual std::pair<bool, Material&> getMaterialOfModel(Model &model) = 0;
        virtual std::pair<bool, IObjectRenderer&> getRenderer(Model &model) = 0;
        virtual void convertQuadIndicesToTriangleIndices(Model &model) = 0;
        virtual void invertNormal(Model &model) = 0;
        virtual void buildInwardCube(Model &model) = 0;
        virtual void buildOutwardCube(Model &model) = 0;
    };
}

#endif //ANDROIDSDK_IMODELSERVICE_H
