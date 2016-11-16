//
// Created by chimeralabs on 10/25/2016.
//

#ifndef ANDROIDSDK_SHADERGLUNIFORMCOLORRENDERER_H
#define ANDROIDSDK_SHADERGLUNIFORMCOLORRENDERER_H

#include <memory>
#include <coreEngine/model/IObjectRenderer.h>
#include <glImplementation/service/IShaderGLService.h>
#include <coreEngine/service/ISceneService.h>
#include <coreEngine/service/IComponentService.h>

namespace cl{
    class ShaderGLUniformColorRenderer : public IObjectRenderer{
    private:
        Logger *loggerPtr;
        std::unique_ptr<IShaderGLService> shaderGLServicePtr;
        std::unique_ptr<ISceneService> sceneServicePtr;
        std::unique_ptr<IComponentService> componentServicePtr;
    public:
        ShaderGLUniformColorRenderer();
        bool initialize(Object &object);
        bool draw(Object &object);
        bool deinitialize(Object &object);
    };
}
#endif //ANDROIDSDK_SHADERGLUNIFORMCOLORRENDERER_H
