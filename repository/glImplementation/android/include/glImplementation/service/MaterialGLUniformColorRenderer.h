//
// Created by chimeralabs on 10/21/2016.
//


#ifndef ANDROIDSDK_MATERIALGLUNIFORMCOLORRENDERER_H
#define ANDROIDSDK_MATERIALGLUNIFORMCOLORRENDERER_H

#include <coreEngine/model/IObjectRenderer.h>
#include <coreEngine/service/IMaterialService.h>

namespace cl{
    class MaterialGLUniformColorRenderer : public IObjectRenderer{
    private:
        Logger *loggerPtr;
        std::unique_ptr<IMaterialService> materialService;
    public:
        MaterialGLUniformColorRenderer();
        bool initialize(Object &object);
        bool draw(Object &object);
        bool deinitialize(Object &object);
    };
}

#endif //ANDROIDSDK_MATERIALGLUNIFORMCOLORRENDERER_H
