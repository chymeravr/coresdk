#ifndef COREENGINE_TRANSFORMTREE_H
#define COREENGINE_TRANSFORMTREE_H

#include <vector>
#include <coreEngine/conf/MathType.h>

namespace cl{
	class TransformTree{
	public:
		void addChild(TransformTree *child);
		bool getIsDirty();
		void setIsDirty(bool isDirty);
		void setParent(TransformTree *parent);
		TransformTree* getParent();
		std::vector<TransformTree*> &getChildren();
		void setLocalPosition(CL_Vec3 localPosition);
		CL_Vec3 getLocalPosition();
		void setLocalRotation(CL_Vec3 localRotation);
		CL_Vec3 getLocalRotation();
		void setLocalScale(CL_Vec3 scale);
		CL_Vec3 getLocalScale();
		void markAllDescendantsDirty();
		/*
		* localTransform = T * Ry * Rx * Rz * S
		*/
		CL_Mat44 &getGlobalTransform();
	protected:
		TransformTree *parent = nullptr;
		std::vector<TransformTree*> children;
		CL_Vec3 localPosition = CL_Vec3(0.0, 0.0, 0.0);
		CL_Vec3 localRotation = CL_Vec3(0.0, 0.0, 0.0);
		CL_Vec3 localScale = CL_Vec3(1.0, 1.0, 1.0);
		bool isDirty = true;
		CL_Mat44 globalTransform;
	};

}

#endif //COREENGINE_TRANSFORMTREE_H