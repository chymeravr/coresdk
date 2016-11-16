//
// Created by chimeralabs on 10/21/2016.
//

#ifndef ANDROIDSDK_OBJECTFACTORYPARAM_H
#define ANDROIDSDK_OBJECTFACTORYPARAM_H

#include <string>
#include <coreEngine/factory/IObjectFactoryParam.h>

namespace cl{
    class ObjectFactoryParam : public IObjectFactoryParam{
    private:
        std::string tag;
    public:
        ObjectFactoryParam(std::string tag){
            this->tag = tag;
        }
        std::string getTag(){
            return tag;
        }

        void setTag(std::string tag){
            this->tag = tag;
        }

    };
}

#endif //ANDROIDSDK_OBJECTFACTORYPARAM_H
