#include <assert.h>
#include <glImplementation/renderObjects/opengles3/ShaderUniformColorGLES3.h>
#include <glImplementation/util/ShaderProgramUtil.h>
#include <glImplementation/renderObjects/opengles3/MaterialUniformColorGLES3.h>
#include <glImplementation/renderObjects/ModelGL.h>
#include <glImplementation/renderObjects/CameraGL.h>

namespace cl{
	ShaderUniformColorGLES3::ShaderUniformColorGLES3(const std::string &sceneId, ILoggerFactory *loggerFactory, Scene *scene) :ShaderUniformColor(sceneId, loggerFactory, scene){
		logger = loggerFactory->createLogger("glImplementation::ShaderUniformColorGLES3: ");
	}
	IRenderable *ShaderUniformColorGLES3::getRenderable(){
		return this;
	}
	/*
	* Here in initialize, set the program id to all those who need it (Model, Material etc.)
	*/
	bool ShaderUniformColorGLES3::initialize(){
		programId = ShaderProgramUtil::createShaderProgram(vertexShaderSrc, fragmentShaderSrc, logger.get());

		//Initializing shader specific parameters for material and model
		std::vector<Relation*> materialRelations = getRelations("material");
		for (auto it = materialRelations.cbegin(); it != materialRelations.cend(); it++){
			MaterialUniformColorGLES3 *material = (MaterialUniformColorGLES3*)(*it);
			material->setColorUniformId(glGetUniformLocation(programId, "uniformColor"));
			std::vector<Relation*> modelRelations = material->getRelations("model");
			for (auto it = modelRelations.cbegin(); it != modelRelations.cend(); it++){
				ModelGL *model = (ModelGL*)(*it);
				model->setModelMatrixId(glGetUniformLocation(programId, "m"));
			}
		}

		//Initializing parameters for camera
		std::vector<Relation*> sceneRelations = getRelations("scene");
		assert(sceneRelations.size() == 1);
		std::vector<Relation*> cameraRelations = sceneRelations[0]->getRelations("camera");
		assert(cameraRelations.size() == 1);
		CameraGL *camera = (CameraGL*)cameraRelations[0];
		CL_GLuint viewMatrixId = glGetUniformLocation(programId, "v");
		CL_GLuint projectionMatrixId = glGetUniformLocation(programId, "p");
		cameraGLContainer = std::unique_ptr<CameraGLContainer>(new CameraGLContainer(viewMatrixId, projectionMatrixId, camera, logger.get()));
		cameraGLContainer->initialize();
		logger->log(LOG_INFO, "Shader: " + sceneId + " initialized");
		return true;
	}
	void ShaderUniformColorGLES3::draw(){
		glUseProgram(programId);
		cameraGLContainer->draw();
	}
	void ShaderUniformColorGLES3::deinitialize(){
		cameraGLContainer->deinitialize();
		cameraGLContainer = nullptr;
		glDeleteProgram(programId);
	}
}