#include <gtest/gtest.h>
#include <coreEngine/factory/defaultImpl/ShaderFactory.h>
#include <coreEngineTest/mocks/ShaderRendererMock.h>

using namespace cl;

TEST(ShaderFactoryTest, TestShaderCreation){
    std::string tag = "shaderTag";
    std::unique_ptr<IShaderFactory> shaderFactoryPtr(new ShaderFactory());
    std::unique_ptr<IShaderRenderer> shaderRendererPtr(new ShaderRendererMock());
    std::unique_ptr<Object> objectPtr = shaderFactoryPtr->create(tag, std::move(shaderRendererPtr));
    EXPECT_NE(nullptr, objectPtr);
    EXPECT_EQ(tag, objectPtr->getTag());
    EXPECT_EQ("shader", objectPtr->getObjectType());
}