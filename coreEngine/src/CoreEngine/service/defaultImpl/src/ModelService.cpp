//
// Created by chimeralabs on 10/26/2016.
//

#include <coreEngine/service/defaultImpl/ModelService.h>
#include <coreEngine/util/LoggerFactory.h>
#include <coreEngine/service/IServicePool.h>

namespace cl{

    ModelService::ModelService() {
        loggerPtr = &LoggerFactory::getInstance().getLoggerInstance("app::coreEngine::ModelService:");
        objectServicePtr = IServicePool::getInstance().getIObjectService();
    }

    std::pair<bool, Material&> ModelService::getMaterialOfModel(Model &model) {
        std::pair<bool, std::vector<Object *>& > materialsPair = objectServicePtr->getLinkedObjectsByObjectType(model, "material");
        if(!materialsPair.first){
            loggerPtr->log(LOG_ERROR, "Materials retrieval failed from model:"+model.getTag());
            return std::pair<bool, Material&>(false, *(Material*)NULL);
        }
        if(materialsPair.second.size() != 1){
            return std::pair<bool, Material&>(false, *(Material*)NULL);
        }else{
            return std::pair<bool, Material&>(true, *(Material*)materialsPair.second[0]);
        }
    }

    std::pair<bool, IObjectRenderer&> ModelService::getRenderer(Model &model) {
        return std::pair<bool, IObjectRenderer&> (true, model.getObjectRenderer());
    }


    void ModelService::convertQuadIndicesToTriangleIndices(Model &model) {
        std::vector<CL_GLuint> &indices = model.getIndices();
        if(model.getPrimitiveMode() == TRIANGLE_MODE){
            return;
        }
        std::vector<CL_GLuint> triIndices(indices.size()*3/2);
        std::vector<CL_GLuint>::iterator it = indices.begin();
        while( it!=indices.end()){ // In each loop consider quad a,b,c,d
            CL_GLuint a = (*it);it++;
            CL_GLuint b = (*it);it++;
            CL_GLuint c = (*it);it++;
            CL_GLuint d = (*it);it++;
            triIndices.push_back(a);triIndices.push_back(b);triIndices.push_back(d);
            triIndices.push_back(b);triIndices.push_back(c);triIndices.push_back(d);
        }
        indices.clear();
        indices = triIndices;
        model.setPrimitiveMode(TRIANGLE_MODE);
    }

    void ModelService::invertNormal(Model &model) {
        std::vector<CL_GLuint> &indices = model.getIndices();
        std::vector<CL_GLuint> newIndices(indices.size());
        if(model.getPrimitiveMode() == QUAD_MODE){
            //if(indices == NULL) return Model();
            for(int i=0; i<indices.size(); i+=4){
                newIndices.push_back(indices[i+3]);
                newIndices.push_back(indices[i+2]);
                newIndices.push_back(indices[i+1]);
                newIndices.push_back(indices[i]);
            }
            indices.clear();
            indices = newIndices;
        }else if(model.getPrimitiveMode() == TRIANGLE_MODE){
            //if(indices == NULL) return Model();
            for(int i=0; i<indices.size(); i+=3){
                newIndices.push_back(indices[i+2]);
                newIndices.push_back(indices[i+1]);
                newIndices.push_back(indices[i]);
            }
            indices.clear();
            indices = newIndices;
        }
    }

    void ModelService::buildInwardCube(Model &model) {
        std::vector<CL_Vec3> &vertices = model.getVertices();
        std::vector<CL_Vec2> &uvs = model.getUvs();
        std::vector<CL_GLuint> &indices = model.getIndices();

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

        model.setPrimitiveMode(QUAD_MODE);
        for(int i=0; i<24; i++){
            indices.push_back(i);
        }

        convertQuadIndicesToTriangleIndices(model);
    }

    void ModelService::buildOutwardCube(Model &model) {
        buildInwardCube(model);
        invertNormal(model);
    }

    float ModelService::linearInterp(float startY, float endY, float startX, float endX,
                                     float targetX) {
        return (endY - startY) * targetX / (endX - startX);
    }

}