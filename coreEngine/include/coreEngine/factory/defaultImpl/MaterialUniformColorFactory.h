//
// Created by chimeralabs on 10/24/2016.
//

#ifndef ANDROIDSDK_MATERIALUNIFORMCOLORFACTORY_H
#define ANDROIDSDK_MATERIALUNIFORMCOLORFACTORY_H

#include <coreEngine/factory/IMaterialFactory.h>
#include <coreEngine/model/MaterialUniformColor.h>
#include <coreEngine/service/IMaterialService.h>

namespace cl{
    class MaterialUniformColorFactory : public IMaterialFactory{
    private:
        std::unique_ptr<IObjectService> objectServicePtr;
    public:
        MaterialUniformColorFactory(std::unique_ptr<IObjectService> objectServicePtr){
            this->objectServicePtr = std::move(objectServicePtr);
        }

        std::unique_ptr<Object> create(std::string tag, Shader* shaderPtr, std::unique_ptr<IMaterialRenderer> materialRendererPtr) override{
            std::unique_ptr<Object> materialPtr(new MaterialUniformColor(std::move(materialRendererPtr), tag));
            objectServicePtr->linkObject(materialPtr.get(), (Object*)shaderPtr);
            return materialPtr;
        }

        void destroy(Material *materialPtr) override {
            assert(materialPtr != nullptr);
            std::vector<Object *> objects = objectServicePtr->getLinkedObjectsByObjectType(materialPtr, "shader");
            assert(objects.size() == 1);
            objectServicePtr->unlinkObject(objects[0], materialPtr);
        }
    };
}

#endif //ANDROIDSDK_MATERIALUNIFORMCOLORFACTORY_H
