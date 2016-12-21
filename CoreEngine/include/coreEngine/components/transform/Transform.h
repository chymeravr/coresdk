#ifndef COREENGINE_TRANSFORM_H
#define COREENGINE_TRANSFORM_H

#include <coreEngine/renderObjects/IComponent.h>
#include <coreEngine/conf/Types.h>

namespace cl{
    class Transform : public IComponent{
    public:
        Transform(){
            position = CL_Vec3(0.0f, 0.0f, 0.0f);
            rotation = CL_Vec3(0.0f, 0.0f, 0.0f);
            scale = CL_Vec3(1.0f, 1.0f, 1.0f);
        }
        std::string getComponentType(){
            return componentType;
        }
        void setPosition(CL_Vec3 position){
            this->position = position;
        }
        CL_Vec3 getPosition(){
            return this->position;
        }
        /*
        * Rotation in euler angles format. Angle to be given in degrees.
        * Order of multiplicatin is Ry * Rx * Rz.
        */
        void setRotation(CL_Vec3 rotation){
            this->rotation = rotation;
        }
        /*
        * Rotation in euler angles format. Angle to be given in degrees.
        * Order of multiplicatin is Ry * Rx * Rz.
        */
        CL_Vec3 getRotation(){
            return this->rotation;
        }
        void setScale(CL_Vec3 scale){
            this->scale = scale;
        }
        CL_Vec3 getScale(){
            return this->scale;
        }

    protected:
        std::string componentType = "transform";
        CL_Vec3 position;
        CL_Vec3 rotation;
        CL_Vec3 scale;
    };
}

#endif //COREENGINE_TRANSFORM_H