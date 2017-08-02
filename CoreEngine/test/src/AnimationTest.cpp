#include <coreEngine/animation/Animation.h>
#include <coreEngineTest/mocks/InterpolatorMock.h>
#include <gtest/gtest.h>
#include <memory>
#include <thread>

namespace cl {
class AnimationTest : public ::testing::Test {
 protected:
  void SetUp() {
    mockInterpolator =
        std::unique_ptr<IInterpolator<float>>(new InterpolatorMock<float>());
    animation =
        std::unique_ptr<Animation<float>>(new Animation<float>(mockAlpha, std::move(mockInterpolator)));
  }
  void TearDown() {}
  float mockAlpha = 0.0f;
  std::unique_ptr<IInterpolator<float>> mockInterpolator;
  std::unique_ptr<Animation<float>> animation;
};

TEST_F(AnimationTest, TestKeyFramePopulation) {
  std::vector<float> keyFrames = {0.0f, 1.0f};
  std::vector<float> keyTimes = {0.0f, 2.0f};

  animation->setKeyFrames(keyFrames, keyTimes);

  animation->start();

  EXPECT_EQ(animation->getKeyFrameCount(), 2);
  EXPECT_EQ(animation->getKeyFrameCount(), 2);
}

TEST_F(AnimationTest, TestUpdate) {
  std::vector<float> keyFrames = {0.0f, 1.0f, 2.0f};
  std::vector<float> keyTimes = {0.0f, 2.0f, 3.0f};
  animation->setKeyFrames(keyFrames, keyTimes);

  animation->start();
  // implicit assumption of single threaded update
  std::this_thread::sleep_for(std::chrono::seconds(1));

  // we can't predict exactly the time at which the update took place. we assume
  // a generous
  // range. this should work > 95% of the time
  animation->update();
  EXPECT_NEAR(mockAlpha, 0.5, 0.01);			// hard coded the update value to be average of two frames
  /*EXPECT_LE(mockAlpha, 0.55);*/
}
}