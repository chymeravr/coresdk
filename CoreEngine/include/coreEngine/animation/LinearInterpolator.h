#ifndef COREENGINE_LINEARINTERPOLATOR_H
#define COREENGINE_LINEARINTERPOLATOR_H

#include <coreEngine/animation/IInterpolator.h>
#include <cassert>

namespace cl {

template <class T>
class LinearInterpolator : public IInterpolator<T> {
 public:
  ~LinearInterpolator(){};
  T interpolate(T startFrame, T endFrame, float startTime, float endTime,
                float currTime) {
    // only interpolate forward in time not backwards
    assert(endTime > startTime);
    auto slope = (endFrame - startFrame) / (endTime - startTime);
    auto result = startFrame + (currTime - startTime) * slope;

    return result;
  }
};
}

#endif  // COREENGINE_LINEARINTERPOLATOR_H