
#include <assert.h>
#include <coreEngine/renderObjects/Camera.h>
#include <coreEngine/renderObjects/Material.h>
#include <coreEngine/renderObjects/Model.h>
#include <coreEngine/renderObjects/Shader.h>
#include <renderer/RendererNoHMD.h>

namespace cl {
bool RendererNoHMD::start() { return true; }
bool RendererNoHMD::initialize(Scene *scene) {
  IRenderable *sceneRenderer = scene->getRenderable();
  sceneRenderer->initialize();
  std::vector<Relation *> cameraRelations = scene->getRelations("camera");
  assert(cameraRelations.size() == 1);
  ((Camera *)cameraRelations[0])->getRenderable()->initialize();

  std::vector<Relation *> shaderRelations = scene->getRelations("shader");
  for (auto it = shaderRelations.cbegin(); it != shaderRelations.cend(); it++) {
    Shader *shader = (Shader *)(*it);
    shader->getRenderable()->initialize();

    std::vector<Relation *> materialRelations =
        shader->getRelations("material");
    for (auto it = materialRelations.cbegin(); it != materialRelations.cend();
         it++) {
      Material *material = (Material *)(*it);
      material->getRenderable()->initialize();

      std::vector<Relation *> modelRelations = material->getRelations("model");
      for (auto it = modelRelations.cbegin(); it != modelRelations.cend();
           it++) {
        Model *model = (Model *)(*it);
        model->getRenderable()->initialize();
      }
    }
  }
  return true;
}
void RendererNoHMD::update() { return; }
void RendererNoHMD::drawInit(Scene *scene) {
  IRenderable *sceneRenderer = scene->getRenderable();
  sceneRenderer->draw();

  std::vector<Relation *> cameraRelations = scene->getRelations("camera");
  assert(cameraRelations.size() == 1);
  ((Camera *)cameraRelations[0])->getRenderable()->draw();

  std::vector<Relation *> shaderRelations = scene->getRelations("shader");
  for (auto it = shaderRelations.cbegin(); it != shaderRelations.cend(); it++) {
    Shader *shader = (Shader *)(*it);
    shader->getRenderable()->draw();

    std::vector<Relation *> materialRelations =
        shader->getRelations("material");
    for (auto it = materialRelations.cbegin(); it != materialRelations.cend();
         it++) {
      Material *material = (Material *)(*it);
      material->getRenderable()->draw();

      std::vector<Relation *> modelRelations = material->getRelations("model");
      for (auto it = modelRelations.cbegin(); it != modelRelations.cend();
           it++) {
        Model *model = (Model *)(*it);
        model->getRenderable()->draw();
      }
    }
  }
  return;
}

void RendererNoHMD::drawLeft(Scene *scene) { return; }

void RendererNoHMD::drawRight(Scene *scene) { return; }

void RendererNoHMD::drawComplete() {}

void RendererNoHMD::deinitialize(Scene *scene) {
  IRenderable *sceneRenderer = scene->getRenderable();
  sceneRenderer->deinitialize();

  std::vector<Relation *> cameraRelations = scene->getRelations("camera");
  assert(cameraRelations.size() == 1);
  ((Camera *)cameraRelations[0])->getRenderable()->deinitialize();

  std::vector<Relation *> shaderRelations = scene->getRelations("shader");
  for (auto it = shaderRelations.cbegin(); it != shaderRelations.cend(); it++) {
    Shader *shader = (Shader *)(*it);
    shader->getRenderable()->deinitialize();

    std::vector<Relation *> materialRelations =
        shader->getRelations("material");
    for (auto it = materialRelations.cbegin(); it != materialRelations.cend();
         it++) {
      Material *material = (Material *)(*it);
      material->getRenderable()->deinitialize();

      std::vector<Relation *> modelRelations = material->getRelations("model");
      for (auto it = modelRelations.cbegin(); it != modelRelations.cend();
           it++) {
        Model *model = (Model *)(*it);
        model->getRenderable()->deinitialize();
      }
    }
  }
}
void RendererNoHMD::stop() { return; }
void RendererNoHMD::pause() {}
void RendererNoHMD::resume() {}
}