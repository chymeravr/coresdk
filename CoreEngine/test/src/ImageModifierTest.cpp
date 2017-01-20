#include <gtest/gtest.h>

#include <coreEngine/modifier/ImageModifier.h>
#include <coreEngineTest/mocks/LoggerMock.h>

namespace cl{
	class ImageModifierTest : public ::testing::Test{
	protected:
		void SetUp(){
		}
		void TearDown(){
		}
		void testHelper(Image *image, unsigned char value1, unsigned char value2){
			EXPECT_NE(nullptr, image);
			EXPECT_NE(nullptr, image->data);
			EXPECT_EQ(1, image->width);
			EXPECT_EQ(1, image->height);
			EXPECT_EQ(2, image->dataSize);
			EXPECT_EQ(value1, image->data.get()[0]);
			EXPECT_EQ(value2, image->data.get()[1]);
		}
	};

	TEST_F(ImageModifierTest, TestConvertSingleCubicImageIntoSix){
		//Testing a 4x3 subimage with 2 channels
		//(1,2)(3,4)(5,6)(7,8)
		//(9,10)(11,12)(13,14)(15,16)
		//(17,18)(19,20)(21,22)(23,24)
		std::unique_ptr<Image> originalImage(new Image);
		originalImage->width = 4;
		originalImage->height = 3;
		originalImage->dataSize = 24;
		unsigned char count = 1;
		originalImage->data = std::unique_ptr<unsigned char>(new unsigned char[24]);
		for (unsigned int i = 0; i < 3; i++){
			for (unsigned int j = 0; j < 4; j++){
				for (unsigned int k = 0; k < 2; k++){
					originalImage->data.get()[(i * 4 + j) * 2 + k] = count++;
				}
			}
		}
		std::unique_ptr<ILogger> logger(new LoggerMock);
		ImageModifier imageModifier(logger.get());
		std::unique_ptr<Image> frontImage(new Image);
		std::unique_ptr<Image> leftImage(new Image);
		std::unique_ptr<Image> backImage(new Image);
		std::unique_ptr<Image> rightImage(new Image);
		std::unique_ptr<Image> topImage(new Image);
		std::unique_ptr<Image> bottomImage(new Image);
		imageModifier.convertSingleCubicImageIntoSix(originalImage.get(), frontImage.get(), leftImage.get(), backImage.get(), rightImage.get(), topImage.get(), bottomImage.get());
		testHelper(frontImage.get(), 11, 12);
		testHelper(leftImage.get(), 9, 10);
		testHelper(backImage.get(), 15, 16);
		testHelper(rightImage.get(), 13, 14);
		testHelper(topImage.get(), 3, 4);
		testHelper(bottomImage.get(), 19, 20);
	}
}