#include <gtest/gtest.h>
#include <coreEngine/components/gazeDetector/GazeDetectorBox.h>
#include <coreEngine/components/transformTree/TransformTreeFactory.h>
#include <coreEngineTest/mocks/LoggerFactoryMock.h>
#include <coreEngine/components/gazeDetector/EventGazeListener.h>

namespace cl{
	class GazeDetectorBoxTest : public ::testing::Test, public EventGazeListener{
	public:
		MOCK_METHOD0(onGazeStarted, void(void));
		MOCK_METHOD0(onGaze, void(void));
		MOCK_METHOD0(onGazeEnded, void(void));
	protected:
		void SetUp(){
			loggerFactory = std::unique_ptr<ILoggerFactory>(new LoggerFactoryMock);
			transformTreeFactory = std::unique_ptr<TransformTreeFactory>(new TransformTreeFactory(loggerFactory.get()));
			gazeDetectorContainer = std::unique_ptr<GazeDetectorContainer>(new GazeDetectorContainer);
		}
		void TearDown(){

		}
		std::unique_ptr<ILoggerFactory> loggerFactory;
		std::unique_ptr<TransformTreeFactory> transformTreeFactory;
		std::unique_ptr<GazeDetectorContainer> gazeDetectorContainer;
		std::unique_ptr<GazeDetectorBox> gazeDetector;
		std::string gazeId = "gaze";
	};

	TEST_F(GazeDetectorBoxTest, TestGetIsGazed){
		std::unique_ptr<TransformTree> shooterTransform = transformTreeFactory->createTransformTree();
		std::unique_ptr<TransformTree> targetTransform = transformTreeFactory->createTransformTree();
		gazeDetector = std::unique_ptr<GazeDetectorBox>(new GazeDetectorBox(gazeId, shooterTransform.get(), targetTransform.get(), (EventGazeListener*)this, gazeDetectorContainer.get(), CL_Vec3(0.0f, 0.0f, 0.0f), CL_Vec3(0.0f, 0.0f, -1.0f), 1.0f, 1.0f, 1.0f));
		//simple in front test
		//with random rotation
		targetTransform->setLocalPosition(CL_Vec3(0.0f, 0.0f, -10.0f));
		targetTransform->setLocalRotation(CL_Vec3(123.0f, 12.0f, 33.0f));
		EXPECT_TRUE(gazeDetector->getIsGazed());

		//exact back of camera test
		targetTransform->setLocalPosition(CL_Vec3(0.0f, 0.0f, 10.0f));
		targetTransform->setLocalRotation(CL_Vec3(12.0f, 120.0f, 33.0f));
		EXPECT_FALSE(gazeDetector->getIsGazed());

		//lookAt on the side of cube. Expected to be a non-gaze. Gaze is considered only when gaze is inside the cube
		targetTransform->setLocalPosition(CL_Vec3(0.5f, 0.0f, -0.5f));
		targetTransform->setLocalRotation(CL_Vec3(0.0f, 0.0f, 0.0f));
		EXPECT_FALSE(gazeDetector->getIsGazed());

		//changing shooter lookat and position
		targetTransform->setLocalPosition(CL_Vec3(0.0f, 0.0f, -10.0f));
		targetTransform->setLocalRotation(CL_Vec3(0.0f, 0.0f, 0.0f));
		shooterTransform->setLocalPosition(CL_Vec3(-10.0f, 0.0f, 0.0f));
		shooterTransform->setLocalRotation(CL_Vec3(0.0f, -45.0f, 0.0f));
		EXPECT_TRUE(gazeDetector->getIsGazed());

		//rotate a bit more
		shooterTransform->setLocalRotation(CL_Vec3(0.0f, -50.0f, 0.0f));
		EXPECT_FALSE(gazeDetector->getIsGazed());


	}
}