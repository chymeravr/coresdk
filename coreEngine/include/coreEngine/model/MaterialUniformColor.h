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
        MaterialUniformColor(std::unique_ptr<IMaterialRenderer> materialRendererPtr,
                             const std::string &tag) : Material(std::move(materialRendererPtr), tag) { }
        const CL_Vec3 &getColor() const {
            return color;
        }
        void setColor(const CL_Vec3 &color) {
            MaterialUniformColor::color = color;
        }
    };
}

#endif //ANDROIDSDK_MATERIALUNIFORMCOLOR_H
