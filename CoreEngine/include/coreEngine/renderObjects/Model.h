#ifndef COREENGINE_MODEL_H
#define COREENGINE_MODEL_H

#include <string>
#include <vector>
#include <coreEngine/conf/Types.h>
#include <coreEngine/renderObjects/Relation.h>
#include <coreEngine/renderObjects/IScenable.h>
#include <coreEngine/renderObjects/IRenderable.h>
#include <coreEngine/renderObjects/IRelationStore.h>

namespace cl{
    class Model : public Relation, public IScenable{
    public:
        Model(const std::string &sceneId, IRelationStore *relationStore);
        virtual IRenderable *getRenderable() = 0;
        
        std::string getType();
        std::string getSceneId();

        void setModelMatrix(const CL_Mat44 &modelMatrix);
        CL_Mat44 getModelMatrix();

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
        std::vector <CL_Vec3> vertices;
        std::vector <CL_Vec2> uvs;
        std::vector <CL_Vec3> normals;
        std::vector <CL_GLuint> indices;
    };
}

#endif //COREENGINE_MODEL_H