//
// Created by chimeralabs on 10/21/2016.
//

#ifndef ANDROIDSDK_MATERIALFACTORYPARAM_H
#define ANDROIDSDK_MATERIALFACTORYPARAM_H

#include <coreEngine/factory/IObjectFactoryParam.h>

namespace cl{
    class MaterialFactoryParam : public IObjectFactoryParam{
    private:
        std::string tag;
        Shader *shaderPtr;
        IObjectRenderer *rendererPtr;
    public:
        MaterialFactoryParam(std::string tag, Shader &shader, IObjectRenderer &objectRenderer){
            this->tag = tag;
            this->shaderPtr = &shader;
            this->rendererPtr = &objectRenderer;
        }

        Shader &getShader() {
            return *shaderPtr;
        }

        void setShader(Shader &shader) {
            shaderPtr = &shader;
        }

        std::string getTag() {
            return tag;
        }

        void setTag(std::string tag) {
            this->tag = tag;
        }

        void setRenderer(IObjectRenderer &renderer){
            this->rendererPtr = &renderer;
        }

        IObjectRenderer &getRenderer(){
            return *rendererPtr;
        }
    };
}

#endif //ANDROIDSDK_MATERIALFACTORYPARAM_H
