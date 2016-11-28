
#ifndef ANDROIDSDK_ICAMERAFACTORY_H
#define ANDROIDSDK_ICAMERAFACTORY_H

#include <memory>
#include <string>
#include <coreEngine/model/Object.h>
#include <coreEngine/model/ICameraRenderer.h>

namespace cl{
    class ICameraFactory{
    public:
        virtual std::unique_ptr<Object> create(std::string tag, std::unique_ptr<ICameraRenderer> cameraRendererPtr) = 0;
        virtual void destroy(Camera *cameraPtr) = 0;
    };
}

#endif  /ANDROIDSDK_ICAMERAFACTORY_H