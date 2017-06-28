#include <gtest/gtest.h>
#include <coreEngine/components/gazeDetector/GazeDetector.h>
#include <coreEngine/components/transformTree/TransformTreeFactory.h>
#include <coreEngineTest/mocks/LoggerFactoryMock.h>
#include <coreEngineTest/mocks/GazeDetectorMock.h>
#include <coreEngine/components/gazeDetector/EventGazeListener.h>

using ::testing::Return;

namespace cl{
	class GazeDetectorTest : public ::testing::Test, public EventGazeListener{
	public:
		MOCK_METHOD0(onGazeStarted, void(void));
		MOCK_METHOD0(onGaze, void(void));
		MOCK_METHOD0(onGazeEnded, void(void));
		MOCK_METHOD0(inFocus, bool(void));
	protected:
		void SetUp(){
			loggerFactory = std::unique_ptr<ILoggerFactory> (new LoggerFactoryMock);
			transformTreeFactory = std::unique_ptr<TransformTreeFactory>(new TransformTreeFactory(loggerFactory.get()));
			shooterTransform = transformTreeFactory->createTransformTree();
			target1Transform = transformTreeFactory->createTransformTree();
			target2Transform = transformTreeFactory->createTransformTree();
			gazeDetectorContainer = std::unique_ptr<GazeDetectorContainer>(new GazeDetectorContainer);
		}
		void TearDown(){

		}
		std::unique_ptr<ILoggerFactory> loggerFactory;
		std::unique_ptr<TransformTreeFactory> transformTreeFactory;
		std::unique_ptr<TransformTree> shooterTransform;
		std::unique_ptr<TransformTree> target1Transform;
		std::unique_ptr<TransformTree> target2Transform;
		std::unique_ptr<GazeDetectorContainer> gazeDetectorContainer;
		std::unique_ptr<GazeDetectorMock> gazeDetector1;
		std::unique_ptr<GazeDetectorMock> gazeDetector2;
		std::string gaze1Id = "gaze1";
		std::string gaze2Id = "gaze2";
	};
	
	TEST_F(GazeDetectorTest, TestInitialize){
		EXPECT_EQ(0, gazeDetectorContainer->totalGazeDetectors);
		EXPECT_EQ(0, gazeDetectorContainer->gazeDetectors.size());
		gazeDetector1 = std::unique_ptr<GazeDetectorMock>(new GazeDetectorMock(gaze1Id, shooterTransform.get(), target1Transform.get(), (EventGazeListener*)this, gazeDetectorContainer.get()));		
		gazeDetector1->initialize();
		EXPECT_EQ(1, gazeDetectorContainer->totalGazeDetectors);
		EXPECT_EQ(1, gazeDetectorContainer->gazeDetectors.size());
		EXPECT_NE(nullptr, gazeDetectorContainer->gazeDetectors[gaze1Id]);
	}
	
	TEST_F(GazeDetectorTest, TestNoGazeSingleTarget){
		gazeDetector1 = std::unique_ptr<GazeDetectorMock>(new GazeDetectorMock(gaze1Id, shooterTransform.get(), target1Transform.get(), (EventGazeListener*)this, gazeDetectorContainer.get()));
		gazeDetector1->initialize();
		EXPECT_CALL(*this, onGazeStarted()).Times(0);
		EXPECT_CALL(*this, onGaze()).Times(0);
		EXPECT_CALL(*this, onGazeEnded()).Times(0);
		ON_CALL((*gazeDetector1.get()), getIsGazed()).WillByDefault(Return(false));
		gazeDetector1->draw();
	}

	TEST_F(GazeDetectorTest, TestGazeSingleTargetSingleCall){
		gazeDetector1 = std::unique_ptr<GazeDetectorMock>(new GazeDetectorMock(gaze1Id, shooterTransform.get(), target1Transform.get(), (EventGazeListener*)this, gazeDetectorContainer.get()));
		gazeDetector1->initialize();
		EXPECT_CALL(*this, onGazeStarted()).Times(1);
		EXPECT_CALL(*this, onGaze()).Times(1);
		EXPECT_CALL(*this, onGazeEnded()).Times(0);
		ON_CALL((*gazeDetector1.get()), getIsGazed()).WillByDefault(Return(true));
		gazeDetector1->draw();
	}

	TEST_F(GazeDetectorTest, TestGazeSingleTargetTwoCall){
		gazeDetector1 = std::unique_ptr<GazeDetectorMock>(new GazeDetectorMock(gaze1Id, shooterTransform.get(), target1Transform.get(), (EventGazeListener*)this, gazeDetectorContainer.get()));
		gazeDetector1->initialize();
		EXPECT_CALL(*this, onGazeStarted()).Times(1);
		EXPECT_CALL(*this, onGaze()).Times(2);
		EXPECT_CALL(*this, onGazeEnded()).Times(0);
		ON_CALL((*gazeDetector1.get()), getIsGazed()).WillByDefault(Return(true));
		gazeDetector1->draw();
		gazeDetector1->draw();
	}

	TEST_F(GazeDetectorTest, TestGazeEndSingleTargetThreeCall){
		gazeDetector1 = std::unique_ptr<GazeDetectorMock>(new GazeDetectorMock(gaze1Id, shooterTransform.get(), target1Transform.get(), (EventGazeListener*)this, gazeDetectorContainer.get()));
		gazeDetector1->initialize();
		EXPECT_CALL(*this, onGazeStarted()).Times(1);
		EXPECT_CALL(*this, onGaze()).Times(2);
		EXPECT_CALL(*this, onGazeEnded()).Times(1);
		ON_CALL((*gazeDetector1.get()), getIsGazed()).WillByDefault(Return(true));
		gazeDetector1->draw();
		gazeDetector1->draw();
		ON_CALL((*gazeDetector1.get()), getIsGazed()).WillByDefault(Return(false));
		gazeDetector1->draw();
	}

	TEST_F(GazeDetectorTest, TestGazeTwoObjectOneBehindOther){
		gazeDetector1 = std::unique_ptr<GazeDetectorMock>(new GazeDetectorMock(gaze1Id, shooterTransform.get(), target1Transform.get(), (EventGazeListener*)this, gazeDetectorContainer.get()));
		gazeDetector2 = std::unique_ptr<GazeDetectorMock>(new GazeDetectorMock(gaze2Id, shooterTransform.get(), target2Transform.get(), (EventGazeListener*)this, gazeDetectorContainer.get()));
		gazeDetector1->initialize();
		gazeDetector2->initialize();

		EXPECT_EQ(2, gazeDetectorContainer->totalGazeDetectors);
		EXPECT_EQ(2, gazeDetectorContainer->gazeDetectors.size());
		EXPECT_NE(nullptr, gazeDetectorContainer->gazeDetectors[gaze1Id]);
		EXPECT_NE(nullptr, gazeDetectorContainer->gazeDetectors[gaze2Id]);

		EXPECT_CALL(*this, onGazeStarted()).Times(1);
		EXPECT_CALL(*this, onGaze()).Times(1);
		EXPECT_CALL(*this, onGazeEnded()).Times(0);
		ON_CALL((*gazeDetector1.get()), getIsGazed()).WillByDefault(Return(true));
		ON_CALL((*gazeDetector1.get()), getDistanceFromShooter()).WillByDefault(Return(4.0f));
		ON_CALL((*gazeDetector2.get()), getIsGazed()).WillByDefault(Return(true));
		ON_CALL((*gazeDetector2.get()), getDistanceFromShooter()).WillByDefault(Return(2.0f));
		gazeDetector1->draw();
		gazeDetector2->draw();
		EXPECT_NE(gazeDetector1.get(), gazeDetectorContainer->currentTarget);
		EXPECT_EQ(gazeDetector2.get(), gazeDetectorContainer->currentTarget);
	}

	TEST_F(GazeDetectorTest, TestGazeObjectSwitch){
		gazeDetector1 = std::unique_ptr<GazeDetectorMock>(new GazeDetectorMock(gaze1Id, shooterTransform.get(), target1Transform.get(), (EventGazeListener*)this, gazeDetectorContainer.get()));
		gazeDetector2 = std::unique_ptr<GazeDetectorMock>(new GazeDetectorMock(gaze2Id, shooterTransform.get(), target2Transform.get(), (EventGazeListener*)this, gazeDetectorContainer.get()));
		gazeDetector1->initialize();
		gazeDetector2->initialize();
		EXPECT_CALL(*this, onGazeStarted()).Times(2);
		EXPECT_CALL(*this, onGaze()).Times(2);
		EXPECT_CALL(*this, onGazeEnded()).Times(1);
		ON_CALL((*gazeDetector1.get()), getIsGazed()).WillByDefault(Return(true));
		ON_CALL((*gazeDetector1.get()), getDistanceFromShooter()).WillByDefault(Return(4.0f));
		ON_CALL((*gazeDetector2.get()), getIsGazed()).WillByDefault(Return(true));
		ON_CALL((*gazeDetector2.get()), getDistanceFromShooter()).WillByDefault(Return(2.0f));
		gazeDetector1->draw();
		gazeDetector2->draw();
		ON_CALL((*gazeDetector2.get()), getIsGazed()).WillByDefault(Return(false));
		gazeDetector1->draw();
		gazeDetector2->draw();
		EXPECT_NE(gazeDetector2.get(), gazeDetectorContainer->currentTarget);
		EXPECT_EQ(gazeDetector1.get(), gazeDetectorContainer->currentTarget);
	}

	TEST_F(GazeDetectorTest, TestDeInitialize){
		gazeDetector1 = std::unique_ptr<GazeDetectorMock>(new GazeDetectorMock(gaze1Id, shooterTransform.get(), target1Transform.get(), (EventGazeListener*)this, gazeDetectorContainer.get()));
		gazeDetector2 = std::unique_ptr<GazeDetectorMock>(new GazeDetectorMock(gaze2Id, shooterTransform.get(), target2Transform.get(), (EventGazeListener*)this, gazeDetectorContainer.get()));
		gazeDetector1->initialize();
		gazeDetector2->initialize();
		gazeDetector1->deinitialize();
		gazeDetector2->deinitialize();
		EXPECT_EQ(0, gazeDetectorContainer->totalGazeDetectors);
		EXPECT_EQ(0, gazeDetectorContainer->gazeDetectors.size());
	}

}