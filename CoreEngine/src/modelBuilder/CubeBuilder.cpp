#include <coreEngine/modelBuilder/CubeBuilder.h>

namespace cl{
    CubeBuilder::CubeBuilder(ModelModifier *modelModifier){
        this->modelModifier = modelModifier;
    }
    void CubeBuilder::buildInwardCube(Model *model){
        std::vector<CL_Vec3> &vertices = model->getVertices();
        std::vector<CL_Vec2> &uvs = model->getUvs();
        std::vector<CL_GLuint> &indices = model->getIndices();

        //front face
        vertices.push_back(CL_Vec3(-1.0, 1.0, -1.0));//left top front
        vertices.push_back(CL_Vec3(-1.0, -1.0, -1.0));//left bottom front
        vertices.push_back(CL_Vec3(1.0, -1.0, -1.0));//right bottom front
        vertices.push_back(CL_Vec3(1.0, 1.0, -1.0));//right top front

        //left face
        vertices.push_back(CL_Vec3(-1.0, 1.0, 1.0));//left top back
        vertices.push_back(CL_Vec3(-1.0, -1.0, 1.0));//left bottom back
        vertices.push_back(CL_Vec3(-1.0, -1.0, -1.0));//left bottom front
        vertices.push_back(CL_Vec3(-1.0, 1.0, -1.0));//left top front

        //back face
        vertices.push_back(CL_Vec3(1.0, 1.0, 1.0));//right top back
        vertices.push_back(CL_Vec3(1.0, -1.0, 1.0));//right bottom back
        vertices.push_back(CL_Vec3(-1.0, -1.0, 1.0));//left bottom back
        vertices.push_back(CL_Vec3(-1.0, 1.0, 1.0));//left top back

        //right face
        vertices.push_back(CL_Vec3(1.0, 1.0, -1.0));//right top front
        vertices.push_back(CL_Vec3(1.0, -1.0, -1.0));//right bottom front
        vertices.push_back(CL_Vec3(1.0, -1.0, 1.0));//right bottom back
        vertices.push_back(CL_Vec3(1.0, 1.0, 1.0));//right top back

        //top face
        vertices.push_back(CL_Vec3(-1.0, 1.0, 1.0));//left top back
        vertices.push_back(CL_Vec3(-1.0, 1.0, -1.0));//left top front
        vertices.push_back(CL_Vec3(1.0, 1.0, -1.0));//right top front
        vertices.push_back(CL_Vec3(1.0, 1.0, 1.0));//right top back

        //bottom face
        vertices.push_back(CL_Vec3(-1.0, -1.0, -1.0));//left bottom front
        vertices.push_back(CL_Vec3(-1.0, -1.0, 1.0));//left bottom back
        vertices.push_back(CL_Vec3(1.0, -1.0, 1.0));//right bottom back
        vertices.push_back(CL_Vec3(1.0, -1.0, -1.0));//right bottom front

        for (int i = 0; i<6; i++){
            uvs.push_back(CL_Vec2(0.0, 1.0));
            uvs.push_back(CL_Vec2(0.0, 0.0));
            uvs.push_back(CL_Vec2(1.0, 0.0));
            uvs.push_back(CL_Vec2(1.0, 1.0));
        }

        /*
        First making cube of quads as it is easier to visualize.
        */
        for (int i = 0; i<24; i++){
            indices.push_back(i);
        }

        /*
        Convert quads of cube into triangles
        */
        modelModifier->convertQuadIndicesToTriangleIndices(model);
    }

    void CubeBuilder::buildOutwardCube(Model *model){
        buildInwardCube(model);
        modelModifier->invertNormal(model);
    }
}