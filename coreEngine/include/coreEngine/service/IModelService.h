//
// Created by chimeralabs on 10/18/2016.
//

#ifndef ANDROIDSDK_IMODELSERVICE_H
#define ANDROIDSDK_IMODELSERVICE_H

#include <coreEngine/model/Material.h>
#include <coreEngine/model/IModelRenderer.h>

namespace cl{
    class Model;

    class IModelService{
    public:
        /**
        * Returns current material that is attached to the model. Returns nullptr if no material attached
        */
        virtual Material* getMaterialOfModel(Model *modelPtr) = 0;
        /**
        * For each triangle in indices, consider vertices 1, 2, 3. If original triangle is 1-2-3 invertNormal makes it 1-3-2
        */
        virtual void invertNormal(Model *modelPtr) = 0;
        /**
        * Builds a cube whose faces have normals inwards. Order of faces in the data are Front, Left, Back, Right, Top, Bottom.
        */
        virtual void buildInwardCube(Model *modelPtr) = 0;
        /**
        * Builds a cube whose faces have normals outwards. Order of faces in the data are Front, Left, Back, Right, Top, Bottom.
        */
        virtual void buildOutwardCube(Model *modelPtr) = 0;
    };
}

#endif //ANDROIDSDK_IMODELSERVICE_H
