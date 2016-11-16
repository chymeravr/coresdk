//
// Created by chimeralabs on 11/2/2016.
//

#ifndef ANDROIDSDK_ITRANSFORM_H
#define ANDROIDSDK_ITRANSFORM_H

#include <coreEngine/model/IComponent.h>
#include <coreEngine/conf/Types.h>
#include <coreEngine/model/Scene.h>

namespace cl{
    class ITransform : public IComponent{
    public:
        virtual void setScene(Scene &scene) = 0;
        virtual void setPosition(CL_Vec3 position) = 0;
        virtual CL_Vec3 getPosition() = 0;
        virtual void setRoation(CL_Vec3 rotation) = 0;
        virtual CL_Vec3 getRotation() = 0;
        virtual void setScale(CL_Vec3 scale) = 0;
        virtual CL_Vec3 getScale() = 0;
    };
}

#endif //ANDROIDSDK_ITRANSFORM_H
