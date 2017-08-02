#ifndef COREENGINE_IINTERPOLATOR_H
#define COREENGINE_IINTERPOLATOR_H

namespace cl {

template <class T>
class IInterpolator {
 public:
  virtual ~IInterpolator(){};
  virtual T interpolate(T startFrame, T endFrame, float startTime,
                        float endTime, float currTime) = 0;
};
}

#endif  // COREENGINE_IINTERPOLATOR_H