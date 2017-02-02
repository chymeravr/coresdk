#include <coreEngine/ui/UIElement.h>

namespace cl{
	void UIElement::addChild(std::string id, std::unique_ptr<UIElement> child){
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