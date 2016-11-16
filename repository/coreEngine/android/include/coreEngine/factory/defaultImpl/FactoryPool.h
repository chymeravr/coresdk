//
// Created by chimeralabs on 10/24/2016.
//

#ifndef ANDROIDSDK_FACTORYPOOL_H
#define ANDROIDSDK_FACTORYPOOL_H

#include <coreEngine/factory/IFactoryPool.h>
#include "MaterialFactory.h"
#include "ModelFactory.h"
#include "SceneFactory.h"
#include "SceneFactoryParam.h"
#include "ShaderFactory.h"
#include "MaterialUniformColorFactory.h"
#include "CameraFactory.h"

namespace cl{
    class FactoryPool : public IFactoryPool{

    public:
        std::unique_ptr<IObjectFactory> getMaterialFactoryImpl(){
            return std::unique_ptr<IObjectFactory>(new MaterialFactory);
        }

        std::unique_ptr<IObjectRenderer> getMaterialRendererImpl(){
            return nullptr;
        }

        std::unique_ptr<IObjectFactory> getModelFactoryImpl(){
            return std::unique_ptr<IObjectFactory>(new ModelFactory);
        }

        std::unique_ptr<IObjectRenderer> getModelRendererImpl(){
            return nullptr;
        }

        std::unique_ptr<IObjectFactory> getSceneFactoryImpl(){
            return std::unique_ptr<IObjectFactory>(new SceneFactory);
        }

        std::unique_ptr<IObjectRenderer> getSceneRendererImpl (){
            return nullptr;
        }

        std::unique_ptr<IObjectFactory> getShaderFactoryImpl(){
            return std::unique_ptr<IObjectFactory>(new ShaderFactory);
        }

        std::unique_ptr<IObjectRenderer> getShaderRendererImpl(){
            return nullptr;
        }

        std::unique_ptr<IObjectFactory> getMaterialUniformColorFactoryImpl(){
            return std::unique_ptr<IObjectFactory>(new MaterialUniformColorFactory);
        }

        std::unique_ptr<IObjectRenderer> getMaterialUniformColorRendererImpl(){
            return nullptr;
        }

        std::unique_ptr<IObjectFactory> getShaderUniformColorFactoryImpl(){
            return nullptr;
        }

        std::unique_ptr<IObjectRenderer> getShaderUniformColorRendererImpl(){
            return nullptr;
        }

        std::unique_ptr<IObjectFactory> getCameraFactoryImpl(){
            return std::unique_ptr<IObjectFactory>(new CameraFactory);
        }

    };
}

#endif //ANDROIDSDK_FACTORYPOOL_H
