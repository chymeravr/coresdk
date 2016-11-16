//
// Created by chimeralabs on 10/21/2016.
//

#ifndef ANDROIDSDK_IOBJECTFACTORYPARAM_H
#define ANDROIDSDK_IOBJECTFACTORYPARAM_H

#include <string>

namespace cl{
    class IObjectFactoryParam{
    public:
        virtual std::string getTag() = 0;
        virtual void setTag(std::string tag) = 0;
    };
}

#endif //ANDROIDSDK_IOBJECTFACTORYPARAM_H
