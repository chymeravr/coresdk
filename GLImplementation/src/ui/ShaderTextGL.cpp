#include <glImplementation/ui/ShaderTextGL.h>
#include <glImplementation/util/ShaderProgramUtil.h>
#include <glImplementation/ui/MaterialTextGL.h>
#include <glImplementation/renderObjects/ModelGL.h>
#include <glImplementation/renderObjects/CameraGL.h>
#include <glImplementation/renderObjects/CameraGLContainer.h>

namespace cl{
	ShaderTextGL::ShaderTextGL(const std::string &sceneId, ILoggerFactory *loggerFactory,
		Scene *scene) : ShaderText(sceneId, loggerFactory, scene){
		logger = loggerFactory->createLogger("glImplementation::ShaderTextGL: ");
	}
	
	IRenderable *ShaderTextGL::getRenderable()
	{
		return this;
	}

	bool ShaderTextGL::initialize()
	{
		programId = ShaderProgramUtil::createShaderProgram(vertexShaderSrc, fragmentShaderSrc, logger.get());

		//Initializing shader specific parameters for material and model
		std::vector<Relation *> materialRelations = getRelations("material");
		for (auto it = materialRelations.cbegin(); it != materialRelations.cend(); it++)
		{
			MaterialTextGL *material = (MaterialTextGL *)(*it);
			material->setTextTextureId(glGetUniformLocation(programId, "textTexture"));
			material->setColorId(glGetUniformLocation(programId, "textColor"));
			std::vector<Relation *> modelRelations = material->getRelations("model");
			for (auto it = modelRelations.cbegin(); it != modelRelations.cend(); it++)
			{
				ModelGL *model = (ModelGL *)(*it);
				model->setModelMatrixId(glGetUniformLocation(programId, "m"));
			}
		}

		//Initializing parameters for camera
		std::vector<Relation *> sceneRelations = getRelations("scene");
		assert(sceneRelations.size() == 1);
		std::vector<Relation *> cameraRelations = sceneRelations[0]->getRelations("camera");
		assert(cameraRelations.size() == 1);
		CameraGL *camera = (CameraGL *)cameraRelations[0];
		CL_GLuint viewMatrixId = glGetUniformLocation(programId, "v");
		CL_GLuint projectionMatrixId = glGetUniformLocation(programId, "p");
		cameraGLContainer = std::unique_ptr<CameraGLContainer>(new CameraGLContainer(viewMatrixId, projectionMatrixId, camera, logger.get()));
		cameraGLContainer->initialize();
		logger->log(LOG_INFO, "Shader: " + sceneId + " initialized");
		return true;
	}
	void ShaderTextGL::draw()
	{
		glUseProgram(programId);
		cameraGLContainer->draw();
	}
	void ShaderTextGL::deinitialize()
	{
		cameraGLContainer->deinitialize();
		cameraGLContainer = nullptr;
		glDeleteProgram(programId);
	}
}