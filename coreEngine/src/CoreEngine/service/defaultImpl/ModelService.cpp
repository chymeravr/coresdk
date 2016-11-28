//
// Created by chimeralabs on 10/26/2016.
//

#include <coreEngine/service/defaultImpl/ModelService.h>
#include <coreEngine/service/IServicePool.h>

namespace cl{

    ModelService::ModelService(ILoggerFactory *loggerFactoryPtr, std::unique_ptr<IObjectService> objectServicePtr) {
        assert(loggerFactoryPtr != nullptr);
        assert(objectServicePtr != nullptr);
        loggerPtr = loggerFactoryPtr->createLogger("app::coreEngine::ModelService:");
        this->objectServicePtr = std::move(objectServicePtr);
    }

    Material* ModelService::getMaterialOfModel(Model *modelPtr) {
        assert(modelPtr != nullptr);
        std::vector<Object *> materialPtrs = objectServicePtr->getLinkedObjectsByObjectType(modelPtr, "material");
        if(materialPtrs.size() != 1){
            return nullptr;
        }else{
            return (Material*)materialPtrs[0];
        }
    }

    void ModelService::convertQuadIndicesToTriangleIndices(Model *modelPtr) {
        std::vector<CL_GLuint> &indices = modelPtr->getIndices();
        std::vector<CL_GLuint> triIndices;
        std::vector<CL_GLuint>::iterator it = indices.begin();
        while( it!=indices.end()){ // In each loop consider quad a,b,c,d
            CL_GLuint a = (*it);it++;
            CL_GLuint b = (*it);it++;
            CL_GLuint c = (*it);it++;
            CL_GLuint d = (*it);it++;
            triIndices.push_back(a);triIndices.push_back(b);triIndices.push_back(d);
            triIndices.push_back(a);triIndices.push_back(c);triIndices.push_back(d);
        }
        indices.clear();
        indices = triIndices;
    }

    void ModelService::invertNormal(Model *modelPtr) {
        std::vector<CL_GLuint> &indices = modelPtr->getIndices();
        for (unsigned int i = 0; i < indices.size(); i += 3){
            CL_GLuint temp = indices[i + 1];
            indices[i + 1] = indices[i + 2];
            indices[i + 2] = temp;
        }
    }

    void ModelService::buildInwardCube(Model *modelPtr) {
        std::vector<CL_Vec3> &vertices = modelPtr->getVertices();
        std::vector<CL_Vec2> &uvs = modelPtr->getUvs();
        std::vector<CL_GLuint> &indices = modelPtr->getIndices();

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

        for(int i=0; i<6; i++){
            uvs.push_back(CL_Vec2(0.0, 1.0));
            uvs.push_back(CL_Vec2(0.0, 0.0));
            uvs.push_back(CL_Vec2(1.0, 0.0));
            uvs.push_back(CL_Vec2(1.0, 1.0));
        }

        /*
        First making cube of quads as it is easier to visualize.
        */
        for(int i=0; i<24; i++){
            indices.push_back(i);
        }

        /*
        Convert quads of cube into triangles
        */
        convertQuadIndicesToTriangleIndices(modelPtr);
    }

    void ModelService::buildOutwardCube(Model *modelPtr) {
        buildInwardCube(modelPtr);
        invertNormal(modelPtr);
    }

    float ModelService::linearInterp(float startY, float endY, float startX, float endX,
                                     float targetX) {
        return (endY - startY) * targetX / (endX - startX);
    }

}