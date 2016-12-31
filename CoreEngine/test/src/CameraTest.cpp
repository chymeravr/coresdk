#include <gtest/gtest.h>
#include <coreEngineTest/mocks/CameraMock.h>
#include <coreEngineTest/mocks/SceneMock.h>
#include <coreEngineTest/mocks/LoggerFactoryMock.h>

namespace cl{
	class CameraTest : public ::testing::Test{
	protected:
		void SetUp(){
			std::unique_ptr<ILoggerFactory> loggerFactory(new LoggerFactoryMock);
			scene = std::unique_ptr<Scene>(new SceneMock(loggerFactory.get(), sceneId));
			camera = std::unique_ptr<Camera>(new CameraMock(cameraId, loggerFactory.get(), scene.get()));
		}
		void TearDown(){
		}
		std::string sceneId = "scene1";
		std::string cameraId = "camera1";
		const std::string cameraType = "camera";
		const std::string sceneType = "scene";
		std::unique_ptr<Scene> scene;
		std::unique_ptr<Camera> camera;
	};

	TEST_F(CameraTest, TestCameraCreation){
		EXPECT_EQ(cameraId, camera->getUniqueIdentifier());
		EXPECT_EQ(cameraType, camera->getType());
		EXPECT_EQ(cameraId, camera->getSceneId());
		std::vector<Relation*> cameras = scene->getRelations(cameraType);
		EXPECT_EQ(1, cameras.size());
		EXPECT_EQ(cameraId, cameras[0]->getUniqueIdentifier());
		std::vector<Relation*> scenes = camera->getRelations(sceneType);
		EXPECT_EQ(1, scenes.size());
		EXPECT_EQ(sceneId, scenes[0]->getUniqueIdentifier());
	}

	TEST_F(CameraTest, TestViewMatrix){
		double precisionError = 0.01;
		camera->setLocation(CL_Vec3(0.0, 0.0, 10.0));
		camera->setLookAtPoint(CL_Vec3(0.0, 0.0, 0.0));
		camera->setUpVector(CL_Vec3(0.0, 1.0, 0.0));
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
		/*Change up vector*/
		camera->setUpVector(CL_Vec3(-1.0, 1.0, 0.0));
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
		camera->setLocation(CL_Vec3(0.0, 0.0, 0.0));
		camera->setLookAtPoint(CL_Vec3(-1.0, 0.0, -1.0));
		camera->setUpVector(CL_Vec3(0.0, 1.0, 0.0));
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

	TEST_F(CameraTest, TestProjectionMatrix){
		double precisionError = 0.01;
		camera->setAspect(1.0);
		camera->setFarPlane(10.0);
		camera->setNearPlane(1.0);
		camera->setFov(1.57f);
		camera->calculateProjectionMatrix();
		{
			const CL_Mat44 &projectionMatrix = camera->getProjectionMatrix();
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
		camera->setAspect(1.6f);
		camera->setFov(0.927f);
		camera->calculateProjectionMatrix();
		{
			const CL_Mat44 &projectionMatrix = camera->getProjectionMatrix();
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