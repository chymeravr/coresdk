//
// Created by chimeralabs on 10/21/2016.
//

#ifndef ANDROIDSDK_MATERIALFACTORY_H
#define ANDROIDSDK_MATERIALFACTORY_H

#include <coreEngine/factory/IMaterialFactory.h>
#include <coreEngine/service/IMaterialService.h>
#include <assert.h>

namespace cl{
    class MaterialFactory : public IMaterialFactory{
    private:
        std::unique_ptr<IObjectService> objectServicePtr;
    public:
        MaterialFactory(std::unique_ptr<IObjectService> objectServicePtr){
            this->objectServicePtr = std::move(objectServicePtr);
        }
        std::unique_ptr<Object> create(std::string tag, Shader* shaderPtr, std::unique_ptr<IMaterialRenderer> materialRendererPtr) {
            std::unique_ptr<Object> materialPtr(new Material(std::move(materialRendererPtr), tag));
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

#endif //ANDROIDSDK_MATERIALFACTORY_H
