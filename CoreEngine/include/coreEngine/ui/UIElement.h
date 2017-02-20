#ifndef COREENGINE_UIELEMENT_H
#define COREENGINE_UIELEMENT_H

#include <string>
#include <memory>
#include <unordered_map>
#include <coreEngine/components/transformTree/TransformTree.h>

namespace cl{
	class UIElement{
	public:
		/*
		* Adds a child UI element. All children of a given parent should have unique/different ids. This function doesn't check for uniqueness.
		* If a new child is asked to be added with an id equal to one of the child's id, then it will be replaced.
		*/
		void addChild(std::string id, std::unique_ptr<UIElement> child);
		/*
		* Removes child with the given id. Nothing happens if a child with the id isn't present.
		*/
		void removeChild(std::string id);
		void setTransformTree(TransformTree *transformTree);
		TransformTree *getTransformTree();
	private:
		TransformTree *transformTree = nullptr;
		std::unordered_map< std::string, std::unique_ptr<UIElement> > children;
		std::unique_ptr<UIElement*> parent = nullptr;
	};
}

#endif //COREENGINE_UIELEMENT_H