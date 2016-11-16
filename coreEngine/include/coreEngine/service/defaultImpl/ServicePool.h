//
// Created by chimeralabs on 10/25/2016.
//


#ifndef ANDROIDSDK_SERVICEPOOL_H
#define ANDROIDSDK_SERVICEPOOL_H

#include <coreEngine/service/IServicePool.h>
#include <coreEngine/service/defaultImpl/SceneService.h>
#include <coreEngine/service/IShaderService.h>
#include <coreEngine/service/defaultImpl/ShaderService.h>
#include <coreEngine/service/IMaterialService.h>
#include <coreEngine/service/defaultImpl/MaterialService.h>
#include <coreEngine/service/IModelService.h>
#include <coreEngine/service/defaultImpl/ModelService.h>
#include <coreEngine/service/ICameraService.h>
#include <coreEngine/service/defaultImpl/CameraService.h>
#include <coreEngine/service/defaultImpl/ObjectService.h>
#include <coreEngine/service/defaultImpl/ComponentService.h>

namespace cl{
    class ServicePool : public IServicePool{
    public:
        std::unique_ptr<ISceneService> getISceneService(){
            return std::unique_ptr<ISceneService>(new SceneService);
        }
        std::unique_ptr<IShaderService> getIShaderService(){
            return std::unique_ptr<IShaderService>(new ShaderService);
        }
        std::unique_ptr<IMaterialService> getIMaterialService(){
            return std::unique_ptr<IMaterialService>(new MaterialService);
        }
        std::unique_ptr<IModelService> getIModelService(){
            return std::unique_ptr<IModelService>(new ModelService);
        }
        std::unique_ptr<ICameraService> getICameraService(){
            return std::unique_ptr<ICameraService>(new CameraService);
        }
        std::unique_ptr<IObjectService> getIObjectService() {
            return std::unique_ptr<IObjectService>(new ObjectService);
        }
        std::unique_ptr<IComponentService> getIComponentService() {
            return std::unique_ptr<IComponentService>(new ComponentService);
        }

    };
}

#endif //ANDROIDSDK_SERVICEPOOL_H
