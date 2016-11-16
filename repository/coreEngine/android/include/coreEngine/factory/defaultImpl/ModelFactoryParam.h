//
// Created by chimeralabs on 10/21/2016.
//

#ifndef ANDROIDSDK_MODELFACTORYPARAM_H
#define ANDROIDSDK_MODELFACTORYPARAM_H

#include <string>
#include <coreEngine/factory/IObjectFactoryParam.h>

namespace cl{
    class ModelFactoryParam : public IObjectFactoryParam{
    private:
        std::string tag;
        IObjectRenderer *rendererPtr;
    public:
        ModelFactoryParam(std::string tag, IObjectRenderer &objectRenderer){
            this->tag = tag;
            this->rendererPtr = &objectRenderer;
        }
        std::string getTag(){
            return tag;
        }

        void setTag(std::string tag){
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

#endif //ANDROIDSDK_MODELFACTORYPARAM_H
