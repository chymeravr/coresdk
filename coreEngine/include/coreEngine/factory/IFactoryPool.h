//
// Created by chimeralabs on 10/24/2016.
//

#ifndef ANDROIDSDK_IFACTORYPOOL_H
#define ANDROIDSDK_IFACTORYPOOL_H

#include <memory>
#include <string>
#include <coreEngine/factory/IObjectFactory.h>
#include <coreEngine/factory/IObjectFactoryParam.h>
#include <coreEngine/model/IObjectRenderer.h>

namespace cl{
    class IFactoryPool{
    public:
        virtual std::unique_ptr<IObjectFactory> getCameraFactoryImpl() = 0;

        virtual std::unique_ptr<IObjectFactory> getMaterialFactoryImpl() = 0;
        virtual std::unique_ptr<IObjectRenderer> getMaterialRendererImpl() = 0;

        virtual std::unique_ptr<IObjectFactory> getModelFactoryImpl() = 0;
        virtual std::unique_ptr<IObjectRenderer> getModelRendererImpl() = 0;

        virtual std::unique_ptr<IObjectFactory> getSceneFactoryImpl() = 0;
        virtual std::unique_ptr<IObjectRenderer> getSceneRendererImpl () = 0;

        virtual std::unique_ptr<IObjectFactory> getShaderFactoryImpl () = 0;
        virtual std::unique_ptr<IObjectRenderer> getShaderRendererImpl() = 0;

        virtual std::unique_ptr<IObjectFactory> getMaterialUniformColorFactoryImpl () = 0;
        virtual std::unique_ptr<IObjectRenderer> getMaterialUniformColorRendererImpl() = 0;

        virtual std::unique_ptr<IObjectFactory> getShaderUniformColorFactoryImpl () = 0;
        virtual std::unique_ptr<IObjectRenderer> getShaderUniformColorRendererImpl() = 0;

    };
}

#endif //ANDROIDSDK_IFACTORYPOOL_H
