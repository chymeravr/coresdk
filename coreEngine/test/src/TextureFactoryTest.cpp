#include <gtest/gtest.h>
#include <coreEngine/factory/defaultImpl/TextureFactory.h>
#include <coreEngineTest/mocks/TextureRendererMock.h>

using namespace cl;

TEST(TextureFactoryTest, TestTextureCreation){
    std::string tag = "textureTag";
    std::unique_ptr<ITextureFactory> textureFactoryPtr(new TextureFactory);
    std::unique_ptr<ITextureRenderer> textureRendererPtr(new TextureRendererMock);
    std::unique_ptr<Object> textureObjectUPtr = textureFactoryPtr->create(tag, std::move(textureRendererPtr));
    EXPECT_NE(nullptr, textureObjectUPtr);
    EXPECT_EQ(tag, textureObjectUPtr->getTag());
    EXPECT_EQ("texture", textureObjectUPtr->getObjectType());
}