#include <assert.h>
#include <coreEngine/components/transform/TransformModel.h>

namespace cl{
	TransformModel::TransformModel(Model *model, ILoggerFactory *loggerFactory){
		assert(model != nullptr);
		this->model = model;
		logger = loggerFactory->createLogger("coreEngine::TransformModel: ");
	}
	IRenderable *TransformModel::getRenderable(){
		return this;
	}
	bool TransformModel::initialize(){
		return true;
	}
	void TransformModel::draw(){
		CL_Mat44 Rx = CL_RotationMatrixX(rotation[0] * CL_PI / 180);
		CL_Mat44 Ry = CL_RotationMatrixY(rotation[1] * CL_PI / 180);
		CL_Mat44 Rz = CL_RotationMatrixZ(rotation[2] * CL_PI / 180);
		CL_Mat44 T = CL_TranlationMatrix(position);
		CL_Mat44 S = CL_ScaleMatrix(scale);
		CL_Mat44 modelMatrix = T * Ry * Rx * Rz * S;
		model->setModelMatrix(modelMatrix);
	}
	void TransformModel::deinitialize(){
	}
}