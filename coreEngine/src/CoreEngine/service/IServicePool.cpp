//
// Created by chimeralabs on 10/28/2016.
//
#include <coreEngine/service/IServicePool.h>

namespace cl{
    std::unique_ptr<IServicePool> IServicePool::servicePoolPointer = nullptr;
}