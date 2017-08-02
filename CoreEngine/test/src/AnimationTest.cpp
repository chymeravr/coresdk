#include <coreEngine/animation/Animation.h>
#include <coreEngineTest/mocks/InterpolatorMock.h>
#include <coreEngineTest/mocks/LoggerFactoryMock.h>
#include <coreEngineTest/mocks/TimeKeeperMock.h>
#include <gtest/gtest.h>

#include <memory>
#include <thread>
#include <iostream>

#include "gtest/gtest.h"
using ::testing::AtLeast;

namespace cl {
class AnimationTest : public ::testing::Test {
 protected:
  void SetUp() {
	  loggerFactory = std::unique_ptr<ILoggerFactory>(new LoggerFactoryMock);
    std::unique_ptr<IInterpolator<float>> mockInterpolator =
        std::unique_ptr<IInterpolator<float>>(new InterpolatorMock<float>());
	timeKeeperMock = std::unique_ptr<TimeKeeperMock>(new TimeKeeperMock);
    animation = std::unique_ptr<Animation<float>>(new Animation<float>(
        *loggerFactory, mockAlpha, std::move(mockInterpolator), std::move(timeKeeperMock)));
  }
  void TearDown() {}
  float mockAlpha = 0.0f;
  std::unique_ptr<Animation<float>> animation;
  std::unique_ptr<ILoggerFactory> loggerFactory;
  //std::unique_ptr<ITimeKeeper> timeKeeperMock;
  std::unique_ptr<TimeKeeperMock> timeKeeperMock;
};

TEST_F(AnimationTest, TestKeyFramePopulation) {

	std::cout << "Mock Start Call Init" << std::endl;

  //EXPECT_CALL(*timeKeeperMock, start).Times(AtLeast(1));

  std::cout << "Mock Start Call End" << std::endl;
  std::vector<float> keyFrames = {0.0f, 1.0f};
  std::vector<float> keyTimes = {0.0f, 2.0f};

  std::cout << "Setting KeyFrames" << std::endl;
  animation->setKeyFrames(keyFrames, keyTimes);

  std::cout << "Starting Animation" << std::endl;
  animation->start();

  std::cout << "Testing invariants" << std::endl;
  EXPECT_EQ(animation->getKeyFrameCount(), 2);
  EXPECT_EQ(animation->getKeyFrameCount(), 2);
}

TEST_F(AnimationTest, TestUpdate) {
  std::vector<float> keyFrames = {0.0f, 1.0f, 2.0f};
  std::vector<float> keyTimes = {0.0f, 2.0f, 3.0f};
  animation->setKeyFrames(keyFrames, keyTimes);

  animation->start();

  animation->update();
  EXPECT_NEAR(mockAlpha, 0.5, 0.01);
  // hard coded the update value to be average of two frames
}
}