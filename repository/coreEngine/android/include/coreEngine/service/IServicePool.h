//
// Created by chimeralabs on 10/25/2016.
//

#ifndef ANDROIDSDK_ISERVICEPOOL_H
#define ANDROIDSDK_ISERVICEPOOL_H

#include <memory>
#include <coreEngine/service/ISceneService.h>
#include <coreEngine/service/IShaderService.h>
#include <coreEngine/service/IMaterialService.h>
#include <coreEngine/service/IModelService.h>
#include <coreEngine/service/ICameraService.h>
#include <coreEngine/service/IObjectService.h>
#include <coreEngine/service/IComponentService.h>

namespace cl{

    class IServicePool{
    private:
        static std::unique_ptr<IServicePool> servicePoolPointer;
    public:
        static void setInstance(std::unique_ptr<IServicePool> servicePoolPointer){
            IServicePool::servicePoolPointer = std::move(servicePoolPointer);
        }
        static IServicePool &getInstance(){
            return *servicePoolPointer.get();
        }
        virtual std::unique_ptr<ISceneService> getISceneService() = 0;
        virtual std::unique_ptr<IShaderService> getIShaderService() = 0;
        virtual std::unique_ptr<IMaterialService> getIMaterialService() = 0;
        virtual std::unique_ptr<IModelService> getIModelService() = 0;
        virtual std::unique_ptr<ICameraService> getICameraService() = 0;
        virtual std::unique_ptr<IObjectService> getIObjectService() = 0;
        virtual std::unique_ptr<IComponentService> getIComponentService() = 0;
    };
}

#endif //ANDROIDSDK_ISERVICEPOOL_H
