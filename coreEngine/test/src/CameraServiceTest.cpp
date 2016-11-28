#include <gtest/gtest.h>
#include <coreEngine/factory/defaultImpl/CameraFactory.h>
#include <coreEngineTest/mocks/CameraRendererMock.h>
#include <coreEngine/service/defaultImpl/CameraService.h>

namespace cl{
    class CameraServiceTest : public ::testing::Test{
    protected:
        Camera *cameraPtr;
        std::unique_ptr<ICameraService> cameraServicePtr;
        virtual void SetUp(){
            std::string cameraTag = "cameratag";
            std::unique_ptr<ICameraFactory> cameraFactoryPtr(new CameraFactory());
            std::unique_ptr<ICameraRenderer> cameraRendererPtr(new CameraRendererMock);
            std::unique_ptr<Object> cameraObjectPtr = cameraFactoryPtr->create(cameraTag, std::move(cameraRendererPtr));
            cameraPtr = (Camera *)cameraObjectPtr.get();
            cameraServicePtr = std::unique_ptr<ICameraService>(new CameraService);
        }
        virtual void TearDown(){

        }
    };
    TEST_F(CameraServiceTest, TestUpdateCameraViewMatrix){
        double precisionError = 0.01;
        cameraPtr->setLocation(CL_Vec3(0.0, 0.0, 10.0));
        cameraPtr->setLookAtPoint(CL_Vec3(0.0, 0.0, 0.0));
        cameraPtr->setUpVector(CL_Vec3(0.0, 1.0, 0.0));
        cameraServicePtr->updateCameraViewMatrix(cameraPtr);
        {
            const CL_Mat44 &viewMatrix = cameraPtr->getViewMatrix();
            EXPECT_NEAR(1.0, viewMatrix[0][0], precisionError);
            EXPECT_NEAR(0.0, viewMatrix[0][1], precisionError);
            EXPECT_NEAR(0.0, viewMatrix[0][2], precisionError);
            EXPECT_NEAR(0.0, viewMatrix[0][3], precisionError);
            EXPECT_NEAR(0.0, viewMatrix[1][0], precisionError);
            EXPECT_NEAR(1.0, viewMatrix[1][1], precisionError);
            EXPECT_NEAR(0.0, viewMatrix[1][2], precisionError);
            EXPECT_NEAR(0.0, viewMatrix[1][3], precisionError);
            EXPECT_NEAR(0.0, viewMatrix[2][0], precisionError);
            EXPECT_NEAR(0.0, viewMatrix[2][1], precisionError);
            EXPECT_NEAR(1.0, viewMatrix[2][2], precisionError);
            EXPECT_NEAR(0.0, viewMatrix[2][3], precisionError);
            EXPECT_NEAR(0.0, viewMatrix[3][0], precisionError);
            EXPECT_NEAR(0.0, viewMatrix[3][1], precisionError);
            EXPECT_NEAR(-10.0, viewMatrix[3][2], precisionError);
            EXPECT_NEAR(1.0, viewMatrix[3][3], precisionError);
        }
        /*Change up vector*/
        cameraPtr->setUpVector(CL_Vec3(-1.0, 1.0, 0.0));
        cameraServicePtr->updateCameraViewMatrix(cameraPtr);
        {
            const CL_Mat44 &viewMatrix = cameraPtr->getViewMatrix();
            EXPECT_NEAR(0.71, viewMatrix[0][0], precisionError);
            EXPECT_NEAR(-0.71, viewMatrix[0][1], precisionError);
            EXPECT_NEAR(0.0, viewMatrix[0][2], precisionError);
            EXPECT_NEAR(0.0, viewMatrix[0][3], precisionError);
            EXPECT_NEAR(0.71, viewMatrix[1][0], precisionError);
            EXPECT_NEAR(0.71, viewMatrix[1][1], precisionError);
            EXPECT_NEAR(0.0, viewMatrix[1][2], precisionError);
            EXPECT_NEAR(0.0, viewMatrix[1][3], precisionError);
            EXPECT_NEAR(0.0, viewMatrix[2][0], precisionError);
            EXPECT_NEAR(0.0, viewMatrix[2][1], precisionError);
            EXPECT_NEAR(1.0, viewMatrix[2][2], precisionError);
            EXPECT_NEAR(0.0, viewMatrix[2][3], precisionError);
            EXPECT_NEAR(0.0, viewMatrix[3][0], precisionError);
            EXPECT_NEAR(0.0, viewMatrix[3][1], precisionError);
            EXPECT_NEAR(-10.0, viewMatrix[3][2], precisionError);
            EXPECT_NEAR(1.0, viewMatrix[3][3], precisionError);
        }
        cameraPtr->setLocation(CL_Vec3(0.0, 0.0, 0.0));
        cameraPtr->setLookAtPoint(CL_Vec3(-1.0, 0.0, -1.0));
        cameraPtr->setUpVector(CL_Vec3(0.0, 1.0, 0.0));
        cameraServicePtr->updateCameraViewMatrix(cameraPtr);
        {
            const CL_Mat44 &viewMatrix = cameraPtr->getViewMatrix();
            EXPECT_NEAR(0.71, viewMatrix[0][0], precisionError);
            EXPECT_NEAR(0.0, viewMatrix[0][1], precisionError);
            EXPECT_NEAR(0.71, viewMatrix[0][2], precisionError);
            EXPECT_NEAR(0.0, viewMatrix[0][3], precisionError);
            EXPECT_NEAR(0.0, viewMatrix[1][0], precisionError);
            EXPECT_NEAR(1, viewMatrix[1][1], precisionError);
            EXPECT_NEAR(0.0, viewMatrix[1][2], precisionError);
            EXPECT_NEAR(0.0, viewMatrix[1][3], precisionError);
            EXPECT_NEAR(-0.71, viewMatrix[2][0], precisionError);
            EXPECT_NEAR(0.0, viewMatrix[2][1], precisionError);
            EXPECT_NEAR(0.71, viewMatrix[2][2], precisionError);
            EXPECT_NEAR(0.0, viewMatrix[2][3], precisionError);
            EXPECT_NEAR(0.0, viewMatrix[3][0], precisionError);
            EXPECT_NEAR(0.0, viewMatrix[3][1], precisionError);
            EXPECT_NEAR(0.0, viewMatrix[3][2], precisionError);
            EXPECT_NEAR(1.0, viewMatrix[3][3], precisionError);
        }
    }
    TEST_F(CameraServiceTest, TestUpdateCameraProjectionMatrix){
        double precisionError = 0.01;
        cameraPtr->setAspect(1.0);
        cameraPtr->setFarPlane(10.0);
        cameraPtr->setNearPlane(1.0);
        cameraPtr->setFov(1.57f);
        cameraServicePtr->updateCameraProjectionMatrix(cameraPtr);
        {
            const CL_Mat44 &projectionMatrix = cameraPtr->getProjectionMatrix();
            EXPECT_NEAR(1.0, projectionMatrix[0][0], precisionError);
            EXPECT_NEAR(0.0, projectionMatrix[0][1], precisionError);
            EXPECT_NEAR(0.0, projectionMatrix[0][2], precisionError);
            EXPECT_NEAR(0.0, projectionMatrix[0][3], precisionError);
            EXPECT_NEAR(0.0, projectionMatrix[1][0], precisionError);
            EXPECT_NEAR(1, projectionMatrix[1][1], precisionError);
            EXPECT_NEAR(0.0, projectionMatrix[1][2], precisionError);
            EXPECT_NEAR(0.0, projectionMatrix[1][3], precisionError);
            EXPECT_NEAR(0.0, projectionMatrix[2][0], precisionError);
            EXPECT_NEAR(0.0, projectionMatrix[2][1], precisionError);
            EXPECT_NEAR(-1.22, projectionMatrix[2][2], precisionError);
            EXPECT_NEAR(-1.0, projectionMatrix[2][3], precisionError);
            EXPECT_NEAR(0.0, projectionMatrix[3][0], precisionError);
            EXPECT_NEAR(0.0, projectionMatrix[3][1], precisionError);
            EXPECT_NEAR(-2.22, projectionMatrix[3][2], precisionError);
            EXPECT_NEAR(0.0, projectionMatrix[3][3], precisionError);
        }
        cameraPtr->setAspect(1.6f);
        cameraPtr->setFov(0.927f);
        cameraServicePtr->updateCameraProjectionMatrix(cameraPtr);
        {
            const CL_Mat44 &projectionMatrix = cameraPtr->getProjectionMatrix();
            EXPECT_NEAR(1.25, projectionMatrix[0][0], precisionError);
            EXPECT_NEAR(0.0, projectionMatrix[0][1], precisionError);
            EXPECT_NEAR(0.0, projectionMatrix[0][2], precisionError);
            EXPECT_NEAR(0.0, projectionMatrix[0][3], precisionError);
            EXPECT_NEAR(0.0, projectionMatrix[1][0], precisionError);
            EXPECT_NEAR(2.0, projectionMatrix[1][1], precisionError);
            EXPECT_NEAR(0.0, projectionMatrix[1][2], precisionError);
            EXPECT_NEAR(0.0, projectionMatrix[1][3], precisionError);
            EXPECT_NEAR(0.0, projectionMatrix[2][0], precisionError);
            EXPECT_NEAR(0.0, projectionMatrix[2][1], precisionError);
            EXPECT_NEAR(-1.22, projectionMatrix[2][2], precisionError);
            EXPECT_NEAR(-1.0, projectionMatrix[2][3], precisionError);
            EXPECT_NEAR(0.0, projectionMatrix[3][0], precisionError);
            EXPECT_NEAR(0.0, projectionMatrix[3][1], precisionError);
            EXPECT_NEAR(-2.22, projectionMatrix[3][2], precisionError);
            EXPECT_NEAR(0.0, projectionMatrix[3][3], precisionError);
        }
    }
}