#include <gtest/gtest.h>
#include <coreEngine/factory/IObjectFactory.h>
#include <coreEngine/factory/defaultImpl/ObjectFactory.h>

using namespace cl;

TEST(ObjectFactoryTest, TestObjectCreation){
	std::string type = "Object";
	std::string tag = "firstTag";
	std::unique_ptr<IObjectFactory> objectFactoryPtr(new ObjectFactory());
	std::unique_ptr<Object> objectPtr = objectFactoryPtr->createObject("firstTag");
	EXPECT_NE(nullptr, objectPtr);
	EXPECT_EQ(type, objectPtr->getObjectType());
	EXPECT_EQ(tag, objectPtr->getTag());
}