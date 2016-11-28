//
// Created by chimeralabs on 10/18/2016.
//

#ifndef ANDROIDSDK_MODEL_H
#define ANDROIDSDK_MODEL_H

#include <coreEngine/conf/Types.h>
#include <coreEngine/model/Object.h>
#include <coreEngine/model/IModelRenderer.h>

namespace cl{
    class Model : public Object {
    protected:
        bool isStatic = true;
        std::vector <CL_Vec3> vertices;
        std::vector <CL_Vec2> uvs;
        std::vector <CL_Vec4> colors;
        std::vector <CL_Vec3> normals;
        std::vector <CL_GLuint> indices;
        std::unique_ptr<IModelRenderer> modelRendererPtr;
    public:
        Model(const std::string &tag, std::unique_ptr<IModelRenderer> modelRendererPtr) : Object("model", tag) {
            this->modelRendererPtr = std::move(modelRendererPtr);
        }

        bool isIsStatic() const {
            return isStatic;
        }

        void setIsStatic(bool isStatic) {
            Model::isStatic = isStatic;
        }

/**
         * Returns reference to vertices. Can be modified by caller. Possibly through a service layer. Hence no setter for this.
         */
        std::vector<CL_Vec3> &getVertices(){
            return vertices;
        }

        /**
         * Returns reference to uvs. Can be modified by caller. Possibly through a service layer. Hence no setter for this.
         */
         std::vector<CL_Vec2> &getUvs()  {
            return uvs;
        }

        /**
         * Returns reference to colors. Can be modified by caller. Possibly through a service layer. Hence no setter for this.
         */
         std::vector<CL_Vec4> &getColors()  {
            return colors;
        }

        /**
         * Returns reference to normals. Can be modified by caller. Possibly through a service layer. Hence no setter for this.
         */
         std::vector<CL_Vec3> &getNormals()  {
            return normals;
        }

        /**
         * Returns reference to indices. Can be modified by caller. Possibly through a service layer. Hence no setter for this.
         */
         std::vector<CL_GLuint> &getIndices()  {
            return indices;
        }

        /**
         * Get renderer for the object. No set for this as it is initialized in constructor only.
         */
        IModelRenderer *getModelRendererPtr(){
            return modelRendererPtr.get();
        }
    };
}

#endif //ANDROIDSDK_MODEL_H
