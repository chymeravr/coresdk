#ifndef ANDROIDSDK_MATERIALDIFFUSETEXTUREFACTORY_H
#define ANDROIDSDK_MATERIALDIFFUSETEXTUREFACTORY_H

#include <coreEngine/factory/IMaterialFactory.h>
#include <coreEngine/service/IObjectService.h>
#include <coreEngine/model/MaterialDiffuseTexture.h>

namespace cl{
    class MaterialDiffuseTextureFactory : public IMaterialFactory{
    private:
        std::unique_ptr<IObjectService> objectServicePtr;
    public:
        MaterialDiffuseTextureFactory(std::unique_ptr<IObjectService> objectServicePtr){
            this->objectServicePtr = std::move(objectServicePtr);
        }

        std::unique_ptr<Object> create(std::string tag, Shader* shaderPtr, std::unique_ptr<IMaterialRenderer> materialRendererPtr) override{
            std::unique_ptr<Object> materialPtr(new MaterialDiffuseTexture(std::move(materialRendererPtr), tag));
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

#endif //ANDROIDSDK_MATERIALDIFFUSETEXTUREFACTORY_H