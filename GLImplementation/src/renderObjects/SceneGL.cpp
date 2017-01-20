#include <glImplementation/renderObjects/SceneGL.h>

namespace cl{
	SceneGL::SceneGL(ILoggerFactory *loggerFactory, std::string id) : Scene(loggerFactory, id){
		logger = loggerFactory->createLogger("glImplementation::SceneGL: ");
	}
	IRenderable *SceneGL::getRenderable(){
		return this;
	}
	void SceneGL::setBackgroundColor(CL_Vec4 color){
		this->color = color;
	}
	void SceneGL::setDepthTest(bool enable){
		this->depthTest = enable;
	}
	bool SceneGL::initialize(){
		glClearColor(color[0], color[1], color[2], color[3]);
		if (depthTest){
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
		}
		else{
			glDisable(GL_DEPTH_TEST);
		}
		glGenVertexArrays(1, &vertexArrayId);
		glBindVertexArray(vertexArrayId);
		logger->log(LOG_INFO, "scene initialized");
		return true;
	}
	void SceneGL::draw(){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void SceneGL::deinitialize(){
		glDeleteVertexArrays(1, &vertexArrayId);
	}
}