//
// Created by chimeralabs on 10/21/2016.
//

#ifndef ANDROIDSDK_SHADERFACTORYPARAM_H
#define ANDROIDSDK_SHADERFACTORYPARAM_H

#include <coreEngine/factory/IObjectFactoryParam.h>
#include <coreEngine/model/IObjectRenderer.h>

namespace cl{
    class ShaderFactoryParam : public IObjectFactoryParam{
    private:
        std::string tag;
        IObjectRenderer *objectRendererPtr;
    public:
        ShaderFactoryParam(std::string tag, IObjectRenderer &objectRenderer) {
            this->objectRendererPtr = &objectRenderer;
            this->tag = tag;
        }

        std::string getTag(){
            return tag;
        }

        void setTag(std::string tag){
            this->tag = tag;
        }

        IObjectRenderer &getObjectRenderer() {
            return *objectRendererPtr;
        }

        void setObjectRenderer(IObjectRenderer &objectRenderer) {
            ShaderFactoryParam::objectRendererPtr = &objectRenderer;
        }

    };
}

#endif //ANDROIDSDK_SHADERFACTORYPARAM_H
