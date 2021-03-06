#ifndef COREENGINE_MODEL_H
#define COREENGINE_MODEL_H

#include <coreEngine/conf/Types.h>
#include <coreEngine/renderObjects/ComponentList.h>
#include <coreEngine/renderObjects/IRenderable.h>
#include <coreEngine/renderObjects/IScenable.h>
#include <coreEngine/renderObjects/Relation.h>
#include <string>
#include <vector>

namespace cl {
/*
* This model definition is based on assumption that triangles are used for faces
* (and not quads) and indices are used by default.
* Any model builder, while can have quads as representation in intermediate
* states, but should finally convert it into triangle
* and index representation.
*/
class Model : public Relation, public IScenable {
 public:
  virtual ~Model() {}
  Model(const std::string &sceneId, ILoggerFactory *loggerFactory)
      : Relation(loggerFactory) {
    this->sceneId = sceneId;
  }
  virtual IRenderable *getRenderable() = 0;

  std::string getType() { return this->type; }
  std::string getUniqueIdentifier() { return this->sceneId; }
  std::string getSceneId() { return this->sceneId; }
  ComponentList &getComponentList() { return componentList; }
  void setModelMatrix(const CL_Mat44 &modelMatrix) {
    this->modelMatrix = modelMatrix;
  }
  CL_Mat44 getModelMatrix() { return modelMatrix; }

  /**
  * Returns reference to vertices. Can be modified by caller. Possibly through a
  * service layer. Hence no setter for this.
  */
  std::vector<CL_Vec3> &getVertices() { return vertices; }

  /**
  * Returns reference to uvs. Can be modified by caller. Possibly through a
  * service layer. Hence no setter for this.
  */
  std::vector<CL_Vec2> &getUvs() { return uvs; }

  /**
  * Returns reference to normals. Can be modified by caller. Possibly through a
  * service layer. Hence no setter for this.
  */
  std::vector<CL_Vec3> &getNormals() { return normals; }

  /**
  * Returns reference to indices. Can be modified by caller. Possibly through a
  * service layer. Hence no setter for this.
  */
  std::vector<CL_GLuint> &getIndices() { return indices; }

  void setIsStatic(bool &isStatic) { this->isStatic = isStatic; }

  bool getIsStatic() { return this->isStatic; }

  bool getIsVisible() { return this->isVisible; }

  void setIsVisible(bool visible) { this->isVisible = visible; }

  inline void setDepthTest(bool enabled) { this->depthTest = enabled; }
  inline void setBlending(bool enabled) { this->blending = enabled; }
  inline void setBackFaceCulling(bool enabled) { this->backFaceCull = enabled; }

 protected:
  std::string sceneId;
  bool isStatic = true;
  bool isVisible = true;
  const std::string type = "model";
  std::vector<CL_Vec3> vertices;
  std::vector<CL_Vec2> uvs;
  std::vector<CL_Vec3> normals;
  std::vector<CL_GLuint> indices;
  CL_Mat44 modelMatrix;
  ComponentList componentList;

  bool depthTest = false;
  bool blending = false;
  bool backFaceCull = false;
};
}

#endif  // COREENGINE_MODEL_H