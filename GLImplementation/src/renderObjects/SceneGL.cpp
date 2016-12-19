#include <glImplementation/renderObjects/SceneGL.h>

namespace cl{
    SceneGL::SceneGL(ILoggerFactory *loggerFactory, std::string id) : Scene(loggerFactory, id){
        logger = loggerFactory->createLogger("glImplementation::SceneGL: ");
    }
    IRenderable *SceneGL::getRenderable(){
        return this;
    }
    bool SceneGL::initialize(){
        glGenVertexArrays(1, &vertexArrayId);
        glBindVertexArray(vertexArrayId);
    }
    void SceneGL::draw(){

    }
    void SceneGL::deinitialize(){
        glDeleteVertexArrays(1, &vertexArrayId);
    }
}