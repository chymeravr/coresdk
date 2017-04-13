#include <coreEngine/ui/Reticle.h>

namespace cl{
	Reticle::Reticle(IUniformColorFactory *uniformColorFactory,
		ITransformTreeFactory *transformTreeFactory,
		TransformTree *parent,
		IModelFactory *modelFactory,
		std::string id,
		Scene *scene,
		CL_Vec4 &color){

		assert(uniformColorFactory != nullptr);
		//assert(parent != nullptr);
		assert(transformTreeFactory);
		assert(scene != nullptr);

		ShaderUniformColor *shader = uniformColorFactory->getShader(scene);
		MaterialUniformColor *material = uniformColorFactory->getMaterial(shader, color);

		std::unique_ptr<Model> modelUptr = modelFactory->create(id);
		assert(modelUptr != nullptr);
		Model *model = modelUptr.get();
		scene->addToScene(std::move(modelUptr));

		ConcentricCircleBuilder concentricCircleBuilder;
		concentricCircleBuilder.buildConcentricCircle(model, innerRadius, outerRadius, nDivisions);
		
		//assign material
		model->createBiRelation(material);

		// std::unique_ptr<TransformTreeModel> transformTreeModel = transformTreeFactory->createTransformTreeModel(model);
		// transformTreeModel->setLocalPosition(CL_Vec3(0.0f, 0.0f, -distanceFromCamera));
		// parent->addChild(transformTreeModel.get());
		// model->getComponentList().addComponent(std::move(transformTreeModel));

		std::unique_ptr<TransformTreeModel> transformTreeModel = transformTreeFactory->createTransformTreeModel(model);
		if (parent == nullptr){
			//Creating local transform
			transformTreeModel->setLocalPosition(CL_Vec3(0.0f, 0.0f, 0.0f));
			this->setTransformTreeModel(transformTreeModel.get());
			ComponentList &componentList = model->getComponentList();
			componentList.addComponent(std::move(transformTreeModel));
		}
		else{
			 transformTreeModel->setLocalPosition(CL_Vec3(0.0f, 0.0f, -distanceFromCamera));
			 this->setTransformTreeModel(transformTreeModel.get());
			 parent->addChild(transformTreeModel.get());
			 model->getComponentList().addComponent(std::move(transformTreeModel));
		}
	}

	void Reticle::setTransformTreeModel(TransformTreeModel *transformTreeModel){
		this->transformTreeModel = transformTreeModel;
	}
	TransformTreeModel* Reticle::getTransformTreeModel(){
		return this->transformTreeModel;
	}
}