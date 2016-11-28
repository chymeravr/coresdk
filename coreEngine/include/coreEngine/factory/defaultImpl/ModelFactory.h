//
// Created by chimeralabs on 10/21/2016.
//

#ifndef ANDROIDSDK_MODELFACTORY_H
#define ANDROIDSDK_MODELFACTORY_H

#include <coreEngine/factory/IModelFactoy.h>
#include <coreEngine/model/Model.h>

namespace cl{
    class ModelFactory : public IModelFactory{

    public:
        std::unique_ptr<Object> create(std::string tag, std::unique_ptr<IModelRenderer> modelRendererPtr){
            return std::unique_ptr<Object>(new Model(tag, std::move(modelRendererPtr)));
        }
        void destroy(Model *model){

        }
    };
}

#endif //ANDROIDSDK_MODELFACTORY_H
