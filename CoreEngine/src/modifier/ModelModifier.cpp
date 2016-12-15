#include <coreEngine/modifier/ModelModifier.h>

namespace cl{
    void ModelModifier::convertQuadIndicesToTriangleIndices(Model *model){
        std::vector<CL_GLuint> &indices = model->getIndices();
        std::vector<CL_GLuint> triIndices;
        std::vector<CL_GLuint>::iterator it = indices.begin();
        while (it != indices.end()){ // In each loop consider quad a,b,c,d
            CL_GLuint a = (*it); it++;
            CL_GLuint b = (*it); it++;
            CL_GLuint c = (*it); it++;
            CL_GLuint d = (*it); it++;
            triIndices.push_back(a); triIndices.push_back(b); triIndices.push_back(c);
            triIndices.push_back(a); triIndices.push_back(c); triIndices.push_back(d);
        }
        indices.clear();
        indices = triIndices;
    }

    void ModelModifier::invertNormal(Model *model){
        std::vector<CL_GLuint> &indices = model->getIndices();
        for (unsigned int i = 0; i < indices.size(); i += 3){
            CL_GLuint temp = indices[i + 1];
            indices[i + 1] = indices[i + 2];
            indices[i + 2] = temp;
        }
    }
}