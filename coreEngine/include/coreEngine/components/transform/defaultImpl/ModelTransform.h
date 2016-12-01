#ifndef ANDROIDSDK_MODELTRANSFORM_H
#define ANDROIDSDK_MODELTRANSFORM_H

#include <coreEngine/components/transform/ITransform.h>
#include <coreEngine/model/IComponent.h>

namespace cl{
    class Object;

    class ModelTransform : public ITransform, public IComponent{
    private:
        std::string componentType = "modelTransform";

    public:
        ModelTransform(Object *objectPtr) : IComponent(objectPtr){}
        std::string getComponentType();
        void initialize();
        void draw();
        void deinitialize();
        void setPosition(CL_Vec3 position);
        CL_Vec3 getPosition();
        void setRoation(CL_Vec3 rotation);
        CL_Vec3 getRotation();
        void setScale(CL_Vec3 scale);
        CL_Vec3 getScale();
    };
}

#endif