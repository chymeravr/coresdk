#include <gtest/gtest.h>
#include <coreEngine/renderObjects/ComponentList.h>
#include <coreEngineTest/mocks/ComponentMock.h>

namespace cl{
	TEST(ComponentListTest, TestGetAddAndRemoveComponent){
		std::unique_ptr<ComponentList> componentList(new ComponentList);
		std::unique_ptr<IComponent> component1(new ComponentMock);
		std::string componentType = component1->getComponentType();
		EXPECT_EQ(nullptr, componentList->getComponent(componentType));
		EXPECT_EQ(0, componentList->getComponents().size());
		componentList->removeComponent(componentType);
		componentList->addComponent(std::move(component1));
		IComponent *componentReturned1 = componentList->getComponent(componentType);
		EXPECT_NE(nullptr, componentReturned1);
		EXPECT_EQ(1, componentList->getComponents().size());
		EXPECT_EQ(componentType, componentReturned1->getComponentType());
		componentList->removeComponent(componentType);
		EXPECT_EQ(nullptr, componentList->getComponent(componentType));
		EXPECT_EQ(0, componentList->getComponents().size());
	}
}