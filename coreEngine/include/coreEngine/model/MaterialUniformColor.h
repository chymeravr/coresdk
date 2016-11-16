//
// Created by chimeralabs on 10/20/2016.
//

#ifndef ANDROIDSDK_MATERIALUNIFORMCOLOR_H
#define ANDROIDSDK_MATERIALUNIFORMCOLOR_H

#include <coreEngine/model/Material.h>
#include <coreEngine/conf/Types.h>

namespace cl{
    class MaterialUniformColor : public Material{
    private:
        CL_Vec3 color;

    public:
        MaterialUniformColor(Shader &shader, IObjectRenderer &objectRenderer,
                             const std::string &tag) : Material(shader, objectRenderer, tag) { }
        CL_Vec3 getColor(){
            return color;
        }
        void setColor(CL_Vec3 color){
            this->color = color;
        }
    };
}

#endif //ANDROIDSDK_MATERIALUNIFORMCOLOR_H
