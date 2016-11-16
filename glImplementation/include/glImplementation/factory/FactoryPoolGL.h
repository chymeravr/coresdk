//
// Created by chimeralabs on 10/24/2016.
//

#ifndef ANDROIDSDK_FACTORYPOOLGL_H
#define ANDROIDSDK_FACTORYPOOLGL_H

#include <coreEngine/factory/IFactoryPool.h>
#include <coreEngine/factory/defaultImpl/FactoryPool.h>
#include <glImplementation/factory/ModelGLFactory.h>
#include <glImplementation/service/ModelGLRenderer.h>
#include <glImplementation/factory/MaterialGLUniformColorFactory.h>
#include <glImplementation/service/MaterialGLUniformColorRenderer.h>
#include <glImplementation/factory/ShaderGLUniformColorFactory.h>
#include <glImplementation/service/ShaderGLUniformColorRenderer.h>
#include <coreEngine/util/LoggerFactory.h>
#include <glImplementation/service/SceneGLRenderer.h>

namespace cl{
    class FactoryPoolGL : public IFactoryPool{
    private:
        Logger *loggerPtr;
        FactoryPool factoryPool;
    public:
        FactoryPoolGL(){
            loggerPtr = &LoggerFactory::getInstance().getLoggerInstance("app:glImplementation::FactoryPoolGL:");
            loggerPtr->log(LOG_INFO, "Instance created");
        }

        std::unique_ptr<IObjectFactory> getCameraFactoryImpl(){
            return factoryPool.getCameraFactoryImpl();
        }

        std::unique_ptr<IObjectFactory> getMaterialFactoryImpl(){
            return factoryPool.getMaterialFactoryImpl();
        }

        std::unique_ptr<IObjectRenderer> getMaterialRendererImpl(){
            return factoryPool.getMaterialRendererImpl();
        }

        std::unique_ptr<IObjectFactory> getModelFactoryImpl(){
            return std::unique_ptr<IObjectFactory>(new ModelGLFactory);
        }

        std::unique_ptr<IObjectRenderer> getModelRendererImpl(){
            return std::unique_ptr<IObjectRenderer>(new ModelGLRenderer);
        }

        std::unique_ptr<IObjectFactory> getSceneFactoryImpl(){
            return factoryPool.getSceneFactoryImpl();
        }

        std::unique_ptr<IObjectRenderer> getSceneRendererImpl (){
            return std::unique_ptr<IObjectRenderer>(new SceneGLRenderer);
        }

        std::unique_ptr<IObjectFactory> getShaderFactoryImpl(){
            return factoryPool.getShaderFactoryImpl();
        }

        std::unique_ptr<IObjectRenderer> getShaderRendererImpl(){
            return factoryPool.getShaderRendererImpl();
        }

        std::unique_ptr<IObjectFactory> getMaterialUniformColorFactoryImpl(){
            return std::unique_ptr<IObjectFactory>(new MaterialGLUniformColorFactory);
        }

        std::unique_ptr<IObjectRenderer> getMaterialUniformColorRendererImpl(){
            return std::unique_ptr<IObjectRenderer>(new MaterialGLUniformColorRenderer);
        }

        std::unique_ptr<IObjectFactory> getShaderUniformColorFactoryImpl(){
            return std::unique_ptr<IObjectFactory>(new ShaderGLUniformColorFactory);
        }

        std::unique_ptr<IObjectRenderer> getShaderUniformColorRendererImpl(){
            return std::unique_ptr<IObjectRenderer>(new ShaderGLUniformColorRenderer);
        }

    };
}

#endif //ANDROIDSDK_FACTORYPOOLGL_H
