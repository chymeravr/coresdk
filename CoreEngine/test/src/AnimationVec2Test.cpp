#include <gtest/gtest.h>
#include <memory>
#include <thread>

#include <coreEngine/animation/Animation.h>
#include <coreEngineTest/mocks/InterpolatorVec2Mock.h>
#include <coreEngine/conf/MathType.h>

namespace cl {
class AnimationVec2Test : public ::testing::Test {
 protected:
  void SetUp() {
    mockInterpolator = std::unique_ptr<IInterpolator<CL_Vec2>>(
		new InterpolatorVec2Mock());
    animation = std::unique_ptr<Animation<CL_Vec2>>(
        new Animation<CL_Vec2>(mockVec2, std::move(mockInterpolator)));
  }
  void TearDown() {}
  CL_Vec2 mockVec2 = CL_Vec2(0.0f, 0.0f);
  std::unique_ptr<IInterpolator<CL_Vec2>> mockInterpolator;
  std::unique_ptr<Animation<CL_Vec2>> animation;
};

TEST_F(AnimationVec2Test, TestKeyFramePopulation) {
  std::vector<CL_Vec2> keyFrames = {CL_Vec2(0.0f, 0.0f), CL_Vec2(1.0f, 1.0f)};
  std::vector<float> keyTimes = {0.0f, 2.0f};

  animation->setKeyFrames(keyFrames, keyTimes);
  animation->start();

  EXPECT_EQ(animation->getKeyFrameCount(), 2);
  EXPECT_EQ(animation->getKeyFrameCount(), 2);
}

TEST_F(AnimationVec2Test, TestUpdate) {
  std::vector<CL_Vec2> keyFrames = {CL_Vec2(0.0f, 0.0f), CL_Vec2(2.0f, 4.0f),
                                    CL_Vec2(4.0f, 8.0f)};
  std::vector<float> keyTimes = {0.0f, 2.0f, 6.0f};
  animation->setKeyFrames(keyFrames, keyTimes);
  animation->start();
  // implicit assumption of single threaded update
  std::this_thread::sleep_for(std::chrono::seconds(1));
  animation->update();
//   EXPECT_GE(mockVec2.x, 1.0);
//   EXPECT_LE(mockVec2.x, 1.1);
//   EXPECT_GE(mockVec2.y, 2.0);
//   EXPECT_LE(mockVec2.y, 2.1);
  EXPECT_NEAR(mockVec2.x, 1.0, 0.01);
  EXPECT_NEAR(mockVec2.y, 2.0, 0.01);

//   std::this_thread::sleep_for(std::chrono::seconds(2));
//   animation->update();
//   EXPECT_GE(mockVec2.x, 2.5);
//   EXPECT_LE(mockVec2.x, 2.7);
//   EXPECT_GE(mockVec2.y, 5.0);
//   EXPECT_LE(mockVec2.y, 5.2);
}
}