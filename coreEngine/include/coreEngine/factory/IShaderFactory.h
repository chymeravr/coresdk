
#ifndef ANDROIDSDK_ISHADERFACTORY_H
#define ANDROIDSDK_ISHADERFACTORY_H

#include <memory>
#include <coreEngine/model/Object.h>
#include <coreEngine/model/Shader.h>
#include <coreEngine/model/IShaderRenderer.h>

namespace cl{
    class IShaderFactory{
    public:
        virtual std::unique_ptr<Object> create(std::string tag, std::unique_ptr<IShaderRenderer> shaderRendererPtr) = 0;
        virtual void destroy(Shader *shaderPtr) = 0;
    };
}

#endif //ANDROIDSDK_ISHADERFACTORY_H