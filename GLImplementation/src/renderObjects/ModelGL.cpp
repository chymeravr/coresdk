#include <glImplementation/renderObjects/ModelGL.h>

namespace cl{
	ModelGL::ModelGL(const std::string &sceneId, ILoggerFactory *loggerFactory) :Model(sceneId, loggerFactory){
		logger = loggerFactory->createLogger("glImplementation::ModelGL: ");
	}

	IRenderable *ModelGL::getRenderable(){
		return this;
	}

	void ModelGL::setModelMatrixId(const CL_GLuint &modelMatrixId){
		this->modelMatrixId = modelMatrixId;
	}

	bool ModelGL::initialize(){
		ComponentList &componentList = getComponentList();
		std::vector<IComponent*> components = componentList.getComponents();
		for (auto it = components.cbegin(); it != components.cend(); it++) {
			IRenderable *renderable = (*it)->getRenderable();
			renderable->initialize();
		}

		if (vertices.size() > 0) {
			createVertexBuffer();
		}
		if (uvs.size() > 0) {
			createUVBuffer();
		}
		if (normals.size() > 0) {
			createNormalBuffer();
		}
		if (indices.size() > 0) {
			createIndexBuffer();
		}
		logger->log(LOG_INFO, "model:" + sceneId + " initialized");
		return true;
	}

	void ModelGL::draw(){
		ComponentList &componentList = getComponentList();
		std::vector<IComponent*> components = componentList.getComponents();
		for (auto it = components.cbegin(); it != components.cend(); it++) {
			IRenderable *renderable = (*it)->getRenderable();
			renderable->draw();
		}

		glUniformMatrix4fv(modelMatrixId, 1, CL_GL_FALSE,
			&modelMatrix[0][0]);
		int i = 0;
		if (glIsBuffer(vertexBufferId)) {
			glEnableVertexAttribArray(i);
			glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
			glVertexAttribPointer(
				i,                                // attribute
				3,                                // size
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				(void *)0                          // array buffer offset
				);
			i++;
		}
		if (glIsBuffer(uvBufferId)) {
			glEnableVertexAttribArray(i);
			glBindBuffer(GL_ARRAY_BUFFER, uvBufferId);
			glVertexAttribPointer(
				i,                                // attribute
				2,                                // size
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				(void *)0                          // array buffer offset
				);
			i++;
		}
		if (glIsBuffer(normalBufferId)) {
			glEnableVertexAttribArray(i);
			glBindBuffer(GL_ARRAY_BUFFER, normalBufferId);
			glVertexAttribPointer(
				i,                                // attribute
				3,                                // size
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				(void *)0                          // array buffer offset
				);
			i++;
		}

		if (glIsBuffer(indexBufferId)) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
			glDrawElements(
				GL_TRIANGLES,      // mode
				indices.size(),    // count
				GL_UNSIGNED_INT,   // type
				(void *)0           // element array buffer offset
				);
		}

		i = 0;
		if (glIsBuffer(vertexBufferId)) glDisableVertexAttribArray(i++);
		if (glIsBuffer(uvBufferId)) glDisableVertexAttribArray(i++);
		if (glIsBuffer(normalBufferId)) glDisableVertexAttribArray(i++);
	}

	void ModelGL::deinitialize(){
		ComponentList &componentList = getComponentList();
		std::vector<IComponent*> components = componentList.getComponents();
		for (auto it = components.cbegin(); it != components.cend(); it++) {
			IRenderable *renderable = (*it)->getRenderable();
			renderable->deinitialize();
		}

		if (glIsBuffer(vertexBufferId)) destroyVertexBuffer();
		if (glIsBuffer(uvBufferId)) destroyUVBuffer();
		if (glIsBuffer(normalBufferId)) destroyNormalBuffer();
		if (glIsBuffer(indexBufferId)) destroyIndexBuffer();
	}

	void ModelGL::createVertexBuffer() {
		glGenBuffers(1, &vertexBufferId);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(CL_Vec3),
			&vertices[0],
			(isStatic) ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
	}

	void ModelGL::useVertexBuffer() {
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	}

	void ModelGL::destroyVertexBuffer() {
		glDeleteBuffers(1, &vertexBufferId);
	}

	void ModelGL::createUVBuffer() {
		glGenBuffers(1, &uvBufferId);
		glBindBuffer(GL_ARRAY_BUFFER, uvBufferId);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(CL_Vec2),
			&uvs[0],
			(isStatic) ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
	}

	void ModelGL::useUVBuffer() {
		glBindBuffer(GL_ARRAY_BUFFER, uvBufferId);
	}

	void ModelGL::destroyUVBuffer() {
		glDeleteBuffers(1, &uvBufferId);
	}

	void ModelGL::createNormalBuffer() {
		glGenBuffers(1, &normalBufferId);
		glBindBuffer(GL_ARRAY_BUFFER, normalBufferId);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(CL_Vec3),
			&normals[0],
			(isStatic) ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
	}

	void ModelGL::useNormalBuffer() {
		glBindBuffer(GL_ARRAY_BUFFER, normalBufferId);
	}

	void ModelGL::destroyNormalBuffer() {
		glDeleteBuffers(1, &normalBufferId);
	}

	void ModelGL::createIndexBuffer() {
		glGenBuffers(1, &indexBufferId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(CL_GLuint),
			&indices[0],
			(isStatic) ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
	}

	void ModelGL::useIndexBuffer() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
	}

	void ModelGL::destroyIndexBuffer() {
		glDeleteBuffers(1, &indexBufferId);
	}
}