#include <glImplementation/renderObjects/SceneGL.h>

namespace cl {
SceneGL::SceneGL(ILoggerFactory *loggerFactory, std::string id)
    : Scene(loggerFactory, id) {
  logger = loggerFactory->createLogger("glImplementation::SceneGL: ");
}
IRenderable *SceneGL::getRenderable() { return this; }
void SceneGL::setBackgroundColor(CL_Vec4 color) { this->color = color; }
void SceneGL::setDepthTest(bool enable) { this->depthTest = enable; }
void SceneGL::setBlending(bool enable) { blending = enable; }
bool SceneGL::initialize() {
  glClearColor(color[0], color[1], color[2], color[3]);

  // we have state management at the model level now ~ this is too general
  // consider removing soon.
  if (depthTest) {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
  } else {
    glDisable(GL_DEPTH_TEST);
  }
  if (blending) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  } else {
    glDisable(GL_BLEND);
  }
  // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // opengl must have a bound vertex array object to render (even if it is
  // empty)
  // this requirement is not there in gles implementations - superbible 7
  // edition pg 20..
  glGenVertexArrays(1, &vertexArrayId);
  glBindVertexArray(vertexArrayId);

  logger->log(LOG_INFO, "scene initialized");
  return true;
}
void SceneGL::draw() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }
void SceneGL::deinitialize() {
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &vertexArrayId);
  // glBindBuffer(GL_ARRAY_BUFFER, 0);
}
}