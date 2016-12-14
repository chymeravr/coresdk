#include <gtest/gtest.h>
#include <coreEngineTest/mocks/TextureMock.h>
#include <coreEngineTest/mocks/SceneMock.h>
#include <coreEngineTest/mocks/LoggerFactoryMock.h>

namespace cl{
    class TextureTest : public ::testing::Test{
    protected:
        void SetUp(){
            std::unique_ptr<ILoggerFactory> loggerFactory(new LoggerFactoryMock);
            texture = std::unique_ptr<Texture>(new TextureMock(textureId, loggerFactory.get()));
        }
        void TearDown(){

        }
        std::string textureId = "texture1";
        const std::string textureType = "texture";
        std::unique_ptr<Texture> texture;
    };

    TEST_F(TextureTest, TestTextureCreation){
        EXPECT_EQ(textureId, texture->getSceneId());
    }

    TEST_F(TextureTest, TestDataAdditionAndClearance){
        unsigned int dataSize = 2;
        std::unique_ptr<unsigned char> dataUPtr(new unsigned char[dataSize]);
        unsigned char *data = dataUPtr.get();
        data[0] = 'a';
        data[1] = 'b';
        texture->setTextureData(std::move(dataUPtr));
        texture->setTextureDataSize(dataSize);

        const unsigned char *receivedData = texture->getTextureData();
        unsigned int receivedDataSize = texture->getTextureDataSize();
        EXPECT_EQ(dataSize, receivedDataSize);
        EXPECT_EQ(data[0], receivedData[0]);
        EXPECT_EQ(data[1], receivedData[1]);

        texture->clearTextureData();
        EXPECT_EQ(0, texture->getTextureDataSize());
        EXPECT_EQ(nullptr, texture->getTextureData());
    }
}