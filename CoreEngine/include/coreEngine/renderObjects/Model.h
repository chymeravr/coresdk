#ifndef COREENGINE_MODEL_H
#define COREENGINE_MODEL_H

#include <string>
#include <vector>
#include <coreEngine/conf/Types.h>
#include <coreEngine/renderObjects/Relation.h>
#include <coreEngine/renderObjects/IScenable.h>
#include <coreEngine/renderObjects/IRenderable.h>

namespace cl{
    class Model : public Relation, public IScenable{
    public:
        virtual ~Model(){}
        Model(const std::string &sceneId, ILoggerFactory *loggerFactory) : Relation(loggerFactory){
            this->sceneId = sceneId;
        }
        virtual IRenderable *getRenderable() = 0;
        
        std::string getType(){
            return this->type;
        }
        std::string getUniqueIdentifier(){
            return this->sceneId;
        }
        std::string getSceneId(){
            return this->sceneId;
        }
        void setModelMatrix(const CL_Mat44 &modelMatrix){
            this->modelMatrix = modelMatrix;
        }
        CL_Mat44 getModelMatrix(){
            return modelMatrix;
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

    protected:
        std::string sceneId;
        std::string type = "model";
        std::vector <CL_Vec3> vertices;
        std::vector <CL_Vec2> uvs;
        std::vector <CL_Vec3> normals;
        std::vector <CL_GLuint> indices;
        CL_Mat44 modelMatrix;
    };
}

#endif //COREENGINE_MODEL_H