//
// Created by chimeralabs on 10/18/2016.
//

#ifndef ANDROIDSDK_MODEL_H
#define ANDROIDSDK_MODEL_H

#include <coreEngine/conf/Types.h>
#include <coreEngine/model/Object.h>
#include <coreEngine/util/Logger.h>
#include <coreEngine/model/IObjectRenderer.h>

namespace cl{
    enum PRIMITIVE_MODE{
        TRIANGLE_MODE,
        QUAD_MODE
    };
    enum RENDERING_MODE{
        VERTEX_RENDERING,
        INDEX_RENDERING
    };

    class Model : public Object {
    protected:
        PRIMITIVE_MODE primitiveMode = TRIANGLE_MODE;
        RENDERING_MODE renderingMode = VERTEX_RENDERING;
        bool isStatic = true;
        std::vector <CL_Vec3> vertices;
        std::vector <CL_Vec2> uvs;
        std::vector <CL_Vec4> colors;
        std::vector <CL_Vec3> normals;
        std::vector <CL_GLuint> indices;
        IObjectRenderer *objectRendererPtr;
    public:
        Model( std::string &tag, IObjectRenderer &objectRenderer) : Object("model", tag) {
            this->objectRendererPtr = &objectRenderer;
        }

        PRIMITIVE_MODE getPrimitiveMode()  {
            return primitiveMode;
        }

        void setPrimitiveMode(PRIMITIVE_MODE primitiveMode) {
            Model::primitiveMode = primitiveMode;
        }

        RENDERING_MODE getRenderingMode()  {
            return renderingMode;
        }

        void setRenderingMode(RENDERING_MODE renderingMode) {
            Model::renderingMode = renderingMode;
        }

        bool getIsStatic()  {
            return isStatic;
        }

        void setIsStatic(bool isStatic) {
            Model::isStatic = isStatic;
        }

        std::vector<CL_Vec3> &getVertices(){
            return vertices;
        }

         std::vector<CL_Vec2> &getUvs()  {
            return uvs;
        }

         std::vector<CL_Vec4> &getColors()  {
            return colors;
        }

         std::vector<CL_Vec3> &getNormals()  {
            return normals;
        }

         std::vector<CL_GLuint> &getIndices()  {
            return indices;
        }

        IObjectRenderer &getObjectRenderer(){
            return *objectRendererPtr;
        }
    };
}

#endif //ANDROIDSDK_MODEL_H
