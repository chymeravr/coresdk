#include <coreEngine/components/transformTree/TransformTreeModel.h>
#include <assert.h>

namespace cl{
	TransformTreeModel::TransformTreeModel(Model *model, ILoggerFactory *loggerFactory){
		assert(model != nullptr);
		this->model = model;
		logger = loggerFactory->createLogger("coreEngine::TransformTreeModel: ");
	}
	std::string TransformTreeModel::getComponentType(){
		return componentType;
	}
	IRenderable *TransformTreeModel::getRenderable(){
		return this;
	}
	bool TransformTreeModel::initialize(){
		return true;
	}
	void TransformTreeModel::draw(){
		CL_Mat44 modelMatrix = getGlobalTransform();
		model->setModelMatrix(modelMatrix);
	}
	void TransformTreeModel::deinitialize(){
	}
}