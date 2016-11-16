//
// Created by chimeralabs on 9/7/2016.
//

#ifndef ANDROIDSDK_IRENDERER_H
#define ANDROIDSDK_IRENDERER_H

#include <coreEngine/model/Scene.h>
#include <coreEngine/service/ISceneService.h>

namespace cl{
    class IRenderer{
    public:
        virtual bool start() = 0;
        virtual bool initialize(Scene &scene) = 0;
        virtual void update() = 0;
        virtual void draw(Scene &scene) = 0;
        virtual void deinitialize(Scene &scene) = 0;
        virtual void destroy() = 0;
        //virtual void setScene(Scene &scene)=0;
    };
}

#endif //ANDROIDSDK_IRENDERER_H
