#include <assert.h>
#include <glImplementation/renderObjects/opengles2/ShaderDiffuseTextureCubeMapGLES2.h>
#include <glImplementation/util/ShaderProgramUtil.h>
#include <glImplementation/renderObjects/opengles2/MaterialDiffuseTextureCubeMapGLES2.h>
#include <glImplementation/renderObjects/ModelGL.h>
#include <glImplementation/renderObjects/CameraGL.h>

namespace cl
{
ShaderDiffuseTextureCubeMapGLES2::ShaderDiffuseTextureCubeMapGLES2(const std::string &sceneId, ILoggerFactory *loggerFactory,
								   Scene *scene) : ShaderDiffuseTextureCubeMap(sceneId, loggerFactory, scene)
{
    logger = loggerFactory->createLogger("glImplementation::ShaderDiffuseTextureGLES2: ");
}
IRenderable *ShaderDiffuseTextureCubeMapGLES2::getRenderable()
{
    return this;
}
/*
		* Here in initialize, set the program id to all those who need it (Model, Material etc.)
		*/
bool ShaderDiffuseTextureCubeMapGLES2::initialize()
{
    programId = ShaderProgramUtil::createShaderProgram(vertexShaderSrc, fragmentShaderSrc, logger.get());

    //Initializing shader specific parameters for material and model
    std::vector<Relation *> materialRelations = getRelations("material");
    for (auto it = materialRelations.cbegin(); it != materialRelations.cend(); it++)
    {
	MaterialDiffuseTextureCubeMapGLES2 *material = (MaterialDiffuseTextureCubeMapGLES2 *)(*it);
	material->setDiffuseTextureId(glGetUniformLocation(programId, "diffuseTexture"));
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
void ShaderDiffuseTextureCubeMapGLES2::draw()
{
    glUseProgram(programId);
    cameraGLContainer->draw();
}
void ShaderDiffuseTextureCubeMapGLES2::deinitialize()
{
    cameraGLContainer->deinitialize();
    cameraGLContainer = nullptr;
    glDeleteProgram(programId);
}
}