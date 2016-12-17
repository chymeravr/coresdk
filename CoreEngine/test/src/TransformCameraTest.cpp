#include <gtest/gtest.h>
#include <coreEngineTest/mocks/CameraMock.h>
#include <coreEngineTest/mocks/SceneMock.h>
#include <coreEngineTest/mocks/LoggerFactoryMock.h>
#include <coreEngine/components/transform/TransformCamera.h>

namespace cl{
    class TransformCameraTest : public ::testing::Test{
    protected:
        void SetUp(){
            std::unique_ptr<ILoggerFactory> loggerFactory(new LoggerFactoryMock);
            scene = std::unique_ptr<Scene>(new SceneMock(loggerFactory.get(), sceneId));
            camera = std::unique_ptr<Camera>(new CameraMock(cameraId, loggerFactory.get(), scene.get()));
            transformCamera = std::unique_ptr<TransformCamera>(new TransformCamera(camera.get(), loggerFactory.get()));
        }
        void TearDown(){

        }
        std::string sceneId = "scene1";
        std::string cameraId = "camera1";
        const std::string cameraType = "camera";
        const std::string sceneType = "scene";
        std::unique_ptr<Scene> scene;
        std::unique_ptr<Camera> camera;
        std::unique_ptr<TransformCamera> transformCamera;
        float precision = 0.01f;
    };

    TEST_F(TransformCameraTest, TestGetComponentType){
        EXPECT_EQ("transform", transformCamera->getComponentType());
    }

    TEST_F(TransformCameraTest, TestDraw){
        double precisionError = 0.01;
        transformCamera->setPosition(CL_Vec3(0.0, 0.0, 10.0));
        transformCamera->setRotation(CL_Vec3(0.0, 0.0, 0.0));
        transformCamera->setScale(CL_Vec3(1.0, 1.0, 1.0));
        transformCamera->draw();
        
        camera->calculateViewMatrix();
        {
            const CL_Mat44 &viewMatrix = camera->getViewMatrix();
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


        transformCamera->setPosition(CL_Vec3(0.0, 0.0, 10.0));
        transformCamera->setRotation(CL_Vec3(0.0, 0.0, 45.0));
        transformCamera->setScale(CL_Vec3(1.0, 1.0, 1.0));
        transformCamera->draw();
        camera->calculateViewMatrix();
        {
            const CL_Mat44 &viewMatrix = camera->getViewMatrix();
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

        transformCamera->setPosition(CL_Vec3(0.0, 0.0, 0.0));
        transformCamera->setRotation(CL_Vec3(0.0, 45.0, 0.0));
        transformCamera->setScale(CL_Vec3(1.0, 1.0, 1.0));
        transformCamera->draw();
        camera->calculateViewMatrix();
        {
            const CL_Mat44 &viewMatrix = camera->getViewMatrix();
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
}