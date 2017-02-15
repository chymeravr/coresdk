#include <coreEngine/ui/UIElement.h>
#include <assert.h>

namespace cl{
	void UIElement::addChild(std::string id, std::unique_ptr<UIElement> child){
		assert(child->getTransformTree() != nullptr);
		assert(this->transformTree != nullptr);
		transformTree->addChild(child->getTransformTree());
		children[id] = std::move(child);
	}
	void UIElement::removeChild(std::string id){
		children.erase(id);
		//TODO: How to handle deletion of transformTree
	}
	void UIElement::setTransformTree(TransformTree *transformTree){
		this->transformTree = transformTree;
	}
	TransformTree *UIElement::getTransformTree(){
		return this->transformTree;
	}
}