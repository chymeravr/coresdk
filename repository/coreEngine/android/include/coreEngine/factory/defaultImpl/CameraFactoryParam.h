//
// Created by chimeralabs on 10/25/2016.
//

#ifndef ANDROIDSDK_CAMERAFACTORYPARAM_H
#define ANDROIDSDK_CAMERAFACTORYPARAM_H

#include <coreEngine/factory/IObjectFactoryParam.h>

namespace cl{
    class CameraFactoryParam : public IObjectFactoryParam{
    std::string tag;
    public:
        CameraFactoryParam(std::string tag){
            this->tag = tag;
        }
        virtual std::string getTag() override {
            return tag;
        }

        virtual void setTag(std::string tag) override {
            this->tag = tag;
        }
    };
}

#endif //ANDROIDSDK_CAMERAFACTORYPARAM_H
