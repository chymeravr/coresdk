//
// Created by chimeralabs on 10/28/2016.
//

#ifndef ANDROIDSDK_IOBJECTRENDERER_H
#define ANDROIDSDK_IOBJECTRENDERER_H

#include <coreEngine/model/Object.h>

namespace cl{
    class IObjectRenderer{
    public:
        virtual bool initialize(Object &object) = 0;
        virtual bool draw(Object &object) = 0;
        virtual bool deinitialize(Object &object) = 0;
    };
}

#endif //ANDROIDSDK_IOBJECTRENDERER_H
