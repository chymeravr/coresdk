#include <gtest/gtest.h>
#include <coreEngine/factory/defaultImpl/CameraFactory.h>
#include <coreEngineTest/mocks/CameraRendererMock.h>

using namespace cl;

TEST(CameraFactoryTest, TestCameraCreation){
    std::string cameraTag = "cameratag";
    std::unique_ptr<ICameraFactory> cameraFactoryPtr(new CameraFactory());
    std::unique_ptr<ICameraRenderer> cameraRendererPtr(new CameraRendererMock);
    std::unique_ptr<Object> cameraObject = cameraFactoryPtr->create(cameraTag, std::move(cameraRendererPtr));
    EXPECT_NE(nullptr, cameraObject);
    EXPECT_EQ(cameraTag, cameraObject->getTag());
    EXPECT_EQ("camera", cameraObject->getObjectType());
}