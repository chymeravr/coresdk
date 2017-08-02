#ifndef COREENGINETEST_INTERPOLATORVEC2MOCK_H
#define COREENGINETEST_INTERPOLATORVEC2MOCK_H

#include <coreEngine/animation/IInterpolator.h>
#include <coreEngine/conf/MathType.h>
#include <gmock/gmock.h>

namespace cl {
class InterpolatorVec2Mock : public IInterpolator<CL_Vec2> {
 public:
  CL_Vec2 interpolate(CL_Vec2 startFrame, CL_Vec2 endFrame, float startTime,
                      float endTime, float currTime) {
    return (startFrame + endFrame) * CL_Vec2(0.5f, 0.5f);
  }
};
}

#endif  // COREENGINETEST_INTERPOLATORVEC2MOCK_H