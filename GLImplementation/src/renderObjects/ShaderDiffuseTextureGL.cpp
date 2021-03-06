#include <assert.h>
#include <glImplementation/renderObjects/CameraGL.h>
#include <glImplementation/renderObjects/MaterialDiffuseTextureGL.h>
#include <glImplementation/renderObjects/ModelGL.h>
#include <glImplementation/renderObjects/ShaderDiffuseTextureGL.h>
#include <glImplementation/util/ShaderProgramUtil.h>

namespace cl {
ShaderDiffuseTextureGL::ShaderDiffuseTextureGL(const std::string &sceneId,
                                               ILoggerFactory *loggerFactory,
                                               Scene *scene)
    : ShaderDiffuseTexture(sceneId, loggerFactory, scene) {
  logger =
      loggerFactory->createLogger("glImplementation::ShaderDiffuseTextureGL: ");
}
IRenderable *ShaderDiffuseTextureGL::getRenderable() { return this; }
/*
        * Here in initialize, set the program id to all those who need it
 * (Model, Material etc.)
        */
bool ShaderDiffuseTextureGL::initialize() {
  if (this->isAlphaChannelEnabled()) {
    this->switchToAlphaShader();
  }
  this->programId = ShaderProgramUtil::createShaderProgram(
      vertexShaderSrc, fragmentShaderSrc, logger.get());

  // Initializing shader specific parameters for material and model
  std::vector<Relation *> materialRelations = getRelations("material");
  for (auto it = materialRelations.cbegin(); it != materialRelations.cend();
       it++) {
    MaterialDiffuseTextureGL *material = (MaterialDiffuseTextureGL *)(*it);
    material->setDiffuseTextureId(
        glGetUniformLocation(this->programId, "diffuseTexture"));
    std::vector<Relation *> modelRelations = material->getRelations("model");
    for (auto it = modelRelations.cbegin(); it != modelRelations.cend(); it++) {
      ModelGL *model = (ModelGL *)(*it);
      model->setModelMatrixId(glGetUniformLocation(this->programId, "m"));
    }
  }

  // Initializing parameters for camera
  std::vector<Relation *> sceneRelations = getRelations("scene");
  assert(sceneRelations.size() == 1);
  std::vector<Relation *> cameraRelations =
      sceneRelations[0]->getRelations("camera");
  assert(cameraRelations.size() == 1);
  CameraGL *camera = (CameraGL *)cameraRelations[0];
  CL_GLuint viewMatrixId = glGetUniformLocation(programId, "v");
  CL_GLuint projectionMatrixId = glGetUniformLocation(programId, "p");

  cameraGLContainer = std::unique_ptr<CameraGLContainer>(new CameraGLContainer(
      viewMatrixId, projectionMatrixId, camera, logger.get()));
  cameraGLContainer->initialize();
  logger->log(LOG_INFO, "Shader: " + sceneId + " initialized");
  return true;
}
void ShaderDiffuseTextureGL::draw() {
  glUseProgram(programId);
  cameraGLContainer->draw();
}
void ShaderDiffuseTextureGL::deinitialize() {
  cameraGLContainer->deinitialize();
  cameraGLContainer = nullptr;
  glDeleteProgram(programId);
}
}