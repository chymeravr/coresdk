#include <gtest/gtest.h>
#include <coreEngine/factory/defaultImpl/ModelFactory.h>
#include <coreEngineTest/mocks/ModelRendererMock.h>

using namespace cl;

TEST(ModelFactoryTest, TestModelCreation){
    std::string tag = "tag";
    std::unique_ptr<IModelFactory> modelFactoryPtr(new ModelFactory());
    std::unique_ptr<IModelRenderer> modelRendererPtr(new ModelRendererMock());
    std::unique_ptr<Object> modelPtr = modelFactoryPtr->create(tag, std::move(modelRendererPtr));
    EXPECT_NE(nullptr, modelPtr);
    EXPECT_EQ(tag, modelPtr->getTag());
}