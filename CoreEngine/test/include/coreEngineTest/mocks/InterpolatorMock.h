#ifndef COREENGINETEST_INTERPOLATORMOCK_H
#define COREENGINETEST_INTERPOLATORMOCK_H

#include <coreEngine/animation/IInterpolator.h>
#include <gmock/gmock.h>

namespace cl {
template <class T>
class InterpolatorMock : public IInterpolator<T> {
 public:
  T interpolate(T startFrame, T endFrame, float startTime, float endTime,
                float currTime) {
    return (startFrame + endFrame) * 0.5;
  }
};
}

#endif  // COREENGINETEST_INTERPOLATORMOCK_H