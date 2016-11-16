//
// Created by chimeralabs on 10/26/2016.
//

#ifndef ANDROIDSDK_SERVICEGLPOOL_H
#define ANDROIDSDK_SERVICEGLPOOL_H

#include <glImplementation/service/IServiceGLPool.h>
#include <coreEngine/service/defaultImpl/ServicePool.h>
#include <glImplementation/service/defaultImpl/ShaderGLService.h>

namespace cl{
    class ServiceGLPool : public IServiceGLPool{
    private:
        ServicePool servicePool;
        Logger *loggerPtr;
    public:
        //ServicePool interface methods
        ServiceGLPool(){
            loggerPtr = &LoggerFactory::getInstance().getLoggerInstance("app::glImplementation::ServiceGLPool:");
            loggerPtr->log(LOG_INFO, "Instance created.");
        }

        std::unique_ptr<ISceneService> getISceneService(){
            return servicePool.getISceneService();
        }

        std::unique_ptr<IShaderService> getIShaderService(){
            return servicePool.getIShaderService();
        }

        std::unique_ptr<IMaterialService> getIMaterialService(){
            return servicePool.getIMaterialService();
        }

        std::unique_ptr<IModelService> getIModelService(){
            return servicePool.getIModelService();
        }

        std::unique_ptr<ICameraService> getICameraService(){
            return servicePool.getICameraService();
        }

        std::unique_ptr<IObjectService> getIObjectService() {
            return servicePool.getIObjectService();
        }

        std::unique_ptr<IComponentService> getIComponentService() {
            return servicePool.getIComponentService();
        }

        // ServiceGLPool interface methods
        std::unique_ptr<IShaderGLService> getIShaderGLService(){
            return std::unique_ptr<IShaderGLService> (new ShaderGLService);
        }
    };
}

#endif //ANDROIDSDK_SERVICEGLPOOL_H
