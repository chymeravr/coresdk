#include <coreEngine/animation/LinearInterpolator.h>
#include <gtest/gtest.h>
#include <memory>
#include <thread>

namespace cl {
class LinearInterpolatorTest : public ::testing::Test {
 protected:
  void SetUp() {
    linearInterpolator = std::unique_ptr<LinearInterpolator<float>>(
        new LinearInterpolator<float>());
  }
  void TearDown() {}
  std::unique_ptr<LinearInterpolator<float>> linearInterpolator;
};

TEST_F(LinearInterpolatorTest, TestLinearInterpolation) {
  std::vector<float> keyFrames = {0.0f, 1.0f};
  std::vector<float> keyTimes = {0.0f, 2.0f};

  auto result1 = linearInterpolator->interpolate(keyFrames[0], keyFrames[1],
                                                keyTimes[0], keyTimes[1], 1.0);

  EXPECT_GE(result1, 0.49);
  EXPECT_LE(result1, 0.51);

  auto result2 = linearInterpolator->interpolate(keyFrames[0], keyFrames[1],
	  keyTimes[0], keyTimes[1], 1.5);

  EXPECT_GE(result2, 0.74);
  EXPECT_LE(result2, 0.76);
}
}