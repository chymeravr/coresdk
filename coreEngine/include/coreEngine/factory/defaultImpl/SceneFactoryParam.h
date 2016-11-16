//
// Created by chimeralabs on 10/21/2016.
//


#ifndef ANDROIDSDK_SCENEFACTORYPARAM_H
#define ANDROIDSDK_SCENEFACTORYPARAM_H

#include <coreEngine/factory/IObjectFactoryParam.h>

namespace cl{
    class SceneFactoryParam : public IObjectFactoryParam{
    private:
        std::string tag;
        IObjectRenderer *sceneRendererPtr;
    public:
        SceneFactoryParam(IObjectRenderer &sceneRenderer, std::string tag){
            this->tag = tag;
            this->sceneRendererPtr = &sceneRenderer;
        }
        std::string getTag() {
            return tag;
        }

        void setTag(std::string tag) {
            this->tag = tag;
        }

        void setRenderer(IObjectRenderer &sceneRenderer){
            this->sceneRendererPtr = &sceneRenderer;
        }

        IObjectRenderer &getRenderer(){
            return *this->sceneRendererPtr;
        }
    };
}

#endif //ANDROIDSDK_SCENEFACTORYPARAM_H
