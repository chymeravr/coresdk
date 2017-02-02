#ifndef COREENGINE_TRANSFORMTREE_H
#define COREENGINE_TRANSFORMTREE_H

#include <vector>
#include <coreEngine/conf/MathType.h>

/**
* TransformTree and it's derivative classes can be used to have a hierarchy of transforms. TransformTree class provides core functionality of tranform hierarchy only.
* It doesn't extends IRenderable or IComponent. Classes that extend TransformTree are expected to implement these interfaces. However, TransformTree itself can be used
* for representing transform nodes that are non-renderable -- like nodes that are created to make definition of renderableObject Transform nodes easier.
*/
namespace cl{
	class TransformTree{
	public:
		void addChild(TransformTree *child);
		std::vector<TransformTree*> &getChildren();
		TransformTree* getParent();
		void setLocalPosition(CL_Vec3 localPosition);
		CL_Vec3 getLocalPosition();
		void setLocalRotation(CL_Vec3 localRotation);
		CL_Vec3 getLocalRotation();
		void setLocalScale(CL_Vec3 scale);
		CL_Vec3 getLocalScale();
		/*
		* localTransform = T * Ry * Rx * Rz * S
		*/
		CL_Mat44 &getGlobalTransform();
		void deleteChild(TransformTree *child);
	private:
		bool getIsDirty();
		void setIsDirty(bool isDirty);
		void setParent(TransformTree *parent, int id); 
		void markAllDescendantsDirty();
		void updateId(int newId);
	protected:
		bool isDirty = true;
	private:
		int id;
		TransformTree *parent = nullptr;
		std::vector<TransformTree*> children;
		CL_Vec3 localPosition = CL_Vec3(0.0, 0.0, 0.0);
		CL_Vec3 localRotation = CL_Vec3(0.0, 0.0, 0.0);
		CL_Vec3 localScale = CL_Vec3(1.0, 1.0, 1.0);
		CL_Mat44 globalTransform;

	};

}

#endif //COREENGINE_TRANSFORMTREE_H