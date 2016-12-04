#ifndef COREENGINE_TRANSFORM_H
#define COREENGINE_TRANSFORM_H

#include <coreEngine/renderObjects/IComponent.h>
#include <coreEngine/conf/Types.h>

namespace cl{
    class Transform : public IComponent{
    public:
        std::string getComponentType();
        void setPosition(CL_Vec3 position);
        CL_Vec3 getPosition();
        void setRoation(CL_Vec3 rotation);
        CL_Vec3 getRotation();
        void setScale(CL_Vec3 scale);
        CL_Vec3 getScale();
    };
}

#endif //COREENGINE_TRANSFORM_H