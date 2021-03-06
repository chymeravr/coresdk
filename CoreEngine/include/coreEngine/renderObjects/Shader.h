#ifndef COREENGINE_SHADER_H
#define COREENGINE_SHADER_H

#include <coreEngine/renderObjects/IRenderable.h>
#include <coreEngine/renderObjects/IScenable.h>
#include <coreEngine/renderObjects/Relation.h>
#include <coreEngine/renderObjects/Scene.h>
#include <string>

namespace cl {
class Shader : public Relation, public IScenable {
 public:
  Shader(const std::string &sceneId, ILoggerFactory *loggerFactory,
         Scene *scene)
      : Relation(loggerFactory) {
    this->sceneId = sceneId;
    this->createBiRelation(scene);
  }
  virtual ~Shader() {}
  virtual IRenderable *getRenderable() = 0;
  std::string getType() { return this->type; }
  std::string getUniqueIdentifier() { return this->sceneId; }
  std::string getSceneId() { return this->sceneId; }

 protected:
  std::string sceneId = "";
  const std::string type = "shader";
};
}

#endif  // COREENGINE_SHADER_H