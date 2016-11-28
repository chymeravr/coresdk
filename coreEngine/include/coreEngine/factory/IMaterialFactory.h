
#ifndef ANDROIDSDK_IMATERIALFACTORY_H
#define ANDROIDSDK_IMATERIALFACTORY_H

#include <memory>
#include <coreEngine/model/Object.h>
#include <coreEngine/model/Material.h>
#include <coreEngine/service/IObjectService.h>
#include <coreEngine/model/IMaterialRenderer.h>

namespace cl{
    class IMaterialFactory{
    public:
        virtual std::unique_ptr<Object> create(std::string tag, Shader* shaderPtr, std::unique_ptr<IMaterialRenderer> materialRendererPtr) = 0;
        virtual void destroy(Material *materialPtr) = 0;
    };
}

#endif //ANDROIDSDK_IMATERIALFACTORY_H