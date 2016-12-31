#ifndef GLIMPLEMENTATION_MODELGL_H
#define GLIMPLEMENTATION_MODELGL_H

#include <memory>
#include <conf/GLTypes.h>
#include <coreEngine/renderObjects/Model.h>
#include <coreEngine/renderObjects/IRenderable.h>

namespace cl{
	class ModelGL : public Model, public IRenderable {
	public:
		ModelGL(const std::string &sceneId, ILoggerFactory *loggerFactory);
		IRenderable *getRenderable();

		void setModelMatrixId(const CL_GLuint &modelMatrixId);//To be set by respective shader
		bool initialize();
		void draw();
		void deinitialize();

	private:
		void createVertexBuffer();
		void useVertexBuffer();
		void destroyVertexBuffer();
		void createUVBuffer();
		void useUVBuffer();
		void destroyUVBuffer();
		void createNormalBuffer();
		void useNormalBuffer();
		void destroyNormalBuffer();
		void createIndexBuffer();
		void useIndexBuffer();
		void destroyIndexBuffer();

		std::unique_ptr<ILogger> logger;
		CL_GLuint modelMatrixId;
		CL_GLuint vertexBufferId;
		CL_GLuint uvBufferId;
		CL_GLuint normalBufferId;
		CL_GLuint indexBufferId;
	};
}

#endif //GLIMPLEMENTATION_MODELGL_H
