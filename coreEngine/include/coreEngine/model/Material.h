//
// Created by chimeralabs on 10/18/2016.
//

#ifndef ANDROIDSDK_MATERIAL_H
#define ANDROIDSDK_MATERIAL_H

#include <memory>
#include <coreEngine/model/Object.h>
#include <coreEngine/model/IMaterialRenderer.h>

namespace cl{
    class Shader;
    class Material : public Object{
    protected:
        std::unique_ptr<IMaterialRenderer> materialRendererPtr;
    public:
        /**
         * Constructor.
         */
        Material(std::unique_ptr<IMaterialRenderer> materialRendererPtr, const std::string &tag) : Object("material", tag){
            this->materialRendererPtr = std::move(materialRendererPtr);
        }

        /**
         * Returns the pointer to renderer for this Object. No setter for this as it is only initialized in constructor.
         */
        IMaterialRenderer* getMaterialRendererPtr(){
            return materialRendererPtr.get();
        }
    };
}

#endif //ANDROIDSDK_MATERIAL_H
