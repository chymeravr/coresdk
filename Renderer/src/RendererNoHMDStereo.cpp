
#include <assert.h>
#include <renderer/RendererNoHMDStereo.h>
#include <coreEngine/renderObjects/Camera.h>
#include <coreEngine/renderObjects/Shader.h>
#include <coreEngine/renderObjects/Material.h>
#include <coreEngine/renderObjects/Model.h>

namespace cl{
	void RendererNoHMDStereo::drawInit(Scene *scene){
		IRenderable *sceneRenderer = scene->getRenderable();
		sceneRenderer->draw();
	}

	void RendererNoHMDStereo::draw(Scene *scene, EYE eye){
		IRenderable *sceneRenderer = scene->getRenderable();
		
		// todo - separate camera by position for each eye
		std::vector<Relation*> cameraRelations = scene->getRelations("camera");
		assert(cameraRelations.size() == 1);
		((Camera*)cameraRelations[0])->getRenderable()->draw();
		
		std::vector<Relation*> shaderRelations = scene->getRelations("shader");
		for (auto it = shaderRelations.cbegin(); it != shaderRelations.cend(); it++){
			Shader* shader = (Shader*)(*it);
			shader->getRenderable()->draw();

			std::vector<Relation*> materialRelations = shader->getRelations("material");
			for (auto it = materialRelations.cbegin(); it != materialRelations.cend(); it++){
				Material *material = (Material*)(*it);
				material->getRenderable()->draw();

				std::vector<Relation*> modelRelations = material->getRelations("model");
				for (auto it = modelRelations.cbegin(); it != modelRelations.cend(); it++){
					Model *model = (Model*)(*it);
					model->getRenderable()->draw();
				}
			}
		}
	}

	void RendererNoHMDStereo::drawComplete(){
	}
}