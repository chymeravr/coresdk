#ifndef ANDROIDSDK_MODELTRANSFORM_H
#define ANDROIDSDK_MODELTRANSFORM_H

#include <coreEngine/components/transform/ITransform.h>
#include <coreEngine/model/IComponent.h>

namespace cl{
    class ModelTransform : public ITransform, public IComponent{

    public:
        ModelTransform();
        std::string getComponentType();
        void initialize();
        void draw();
        void deinitialize();
        void setObject(Object *objectPtr);
        Object *getObject();
        void setPosition(CL_Vec3 position);
        CL_Vec3 getPosition();
        void setRoation(CL_Vec3 rotation);
        CL_Vec3 getRotation();
        void setScale(CL_Vec3 scale);
        CL_Vec3 getScale();
    };
}

#endif