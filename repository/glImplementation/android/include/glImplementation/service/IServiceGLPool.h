//
// Created by chimeralabs on 10/25/2016.
//

#ifndef ANDROIDSDK_ISERVICEGLPOOL_H
#define ANDROIDSDK_ISERVICEGLPOOL_H

#include <coreEngine/service/IServicePool.h>
#include <glImplementation/service/IShaderGLService.h>

namespace cl{
    class IServiceGLPool : public IServicePool{
    private:
        static std::unique_ptr<IServiceGLPool> serviceGLPoolPtr;
    public:
        static void setInstance(std::unique_ptr<IServiceGLPool> serviceGLPoolPtr){
            IServiceGLPool::serviceGLPoolPtr = std::move(serviceGLPoolPtr);
        }
        static IServiceGLPool &getInstance(){
            return *serviceGLPoolPtr.get();
        }
        virtual std::unique_ptr<IShaderGLService> getIShaderGLService() = 0;
    };
}

#endif //ANDROIDSDK_ISERVICEGLPOOL_H
