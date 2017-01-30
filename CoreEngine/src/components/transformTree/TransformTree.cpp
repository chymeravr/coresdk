#include <coreEngine/components/transformTree/TransformTree.h>
#include <assert.h>

namespace cl{

	void TransformTree::addChild(TransformTree *child){
		assert(child != nullptr);
		child->setParent(this);
		children.push_back(child);
	}

	bool TransformTree::getIsDirty(){
		return isDirty;
	}

	void TransformTree::setIsDirty(bool isDirty){
		this->isDirty = isDirty;
	}

	void TransformTree::setParent(TransformTree *parent){
		this->parent = parent;
	}

	TransformTree* TransformTree::getParent(){
		return parent;
	}

	std::vector<TransformTree*> &TransformTree::getChildren(){
		return children;
	}

	void TransformTree::setLocalPosition(CL_Vec3 localPosition){
		this->localPosition = localPosition;
		markAllDescendantsDirty();
	}

	CL_Vec3 TransformTree::getLocalPosition(){
		return this->localPosition;
	}

	void TransformTree::setLocalRotation(CL_Vec3 localRotation){
		this->localRotation = localRotation;
		markAllDescendantsDirty();
	}

	CL_Vec3 TransformTree::getLocalRotation(){
		return this->localRotation;
	}

	void TransformTree::setLocalScale(CL_Vec3 scale){
		this->localScale = localScale;
		markAllDescendantsDirty();
	}

	CL_Vec3 TransformTree::getLocalScale(){
		return this->localScale;
	}

	void TransformTree::markAllDescendantsDirty(){
		setIsDirty(true);
		for (auto it = children.cbegin(); it != children.cend(); it++){
			(*it)->markAllDescendantsDirty();
		}
	}

	CL_Mat44 &TransformTree::getGlobalTransform(){
		if (!isDirty)
			return globalTransform;

		CL_Mat44 Rx = CL_RotationMatrixX(localRotation[0] * CL_PI / 180);
		CL_Mat44 Ry = CL_RotationMatrixY(localRotation[1] * CL_PI / 180);
		CL_Mat44 Rz = CL_RotationMatrixZ(localRotation[2] * CL_PI / 180);
		CL_Mat44 T = CL_TranlationMatrix(localPosition);
		CL_Mat44 S = CL_ScaleMatrix(localScale);
		CL_Mat44 localTransform = T * Ry * Rx * Rz * S;

		if (parent == nullptr){
			globalTransform = CL_IdentityMatrix4X4() * localTransform;
		}
		else{
			globalTransform = parent->getGlobalTransform() * localTransform;
		}

		isDirty = false;
		return globalTransform;
	}
}