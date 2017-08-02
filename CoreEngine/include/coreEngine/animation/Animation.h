#ifndef COREENGINE_ANIMATION_H
#define COREENGINE_ANIMATION_H

#include <assert.h>
#include <coreEngine/animation/IInterpolator.h>
#include <chrono>
#include <iostream>
#include <memory>
#include <vector>

// todo - throw warnings if keyframes and keytimes not populated at start /
// update
// can we abstract out the time system here? it makes testing easier?

namespace cl {
template <class T>
class Animation {
 public:
  Animation(T &frameBase, std::unique_ptr<IInterpolator<T>> interpolator) {
    assert(interpolator != nullptr);
    if(interpolator == nullptr){
      throw std::invalid_argument("Interpolator supplied is null");
    }
    this->frameBase = &frameBase;
    this->interpolator = std::move(interpolator);
  }

  ~Animation() {}

  void setKeyFrames(std::vector<T> keyFrames, std::vector<float> keyTimes) {
    // todo - error handling. invariant definitions blah blah
    assert(keyFrames.size() == keyTimes.size());
    for (int i = 0; i < keyFrames.size(); i++) {
      this->keyFrames.push_back(keyFrames[i]);
      this->keyTimes.push_back(keyTimes[i]);
    }
  }

  // start the animations - set start time by fetching the system time
  void start() {
    this->startTime = std::chrono::high_resolution_clock::now();
    this->currTime = this->startTime;
  }

  void update() {
    this->currTime = std::chrono::system_clock::now();
    std::chrono::duration<float> cDiff = this->currTime - this->startTime;
    auto diff = (float)cDiff.count();

    auto startIndex = this->getCurrLowTimeIndex(diff);
    assert(startIndex > -1 && startIndex < this->keyTimes.size());

    auto nextIndex = startIndex + 1;
    if (nextIndex == this->keyTimes.size() - 1) {
      auto nextIndex = this->keyTimes.size() - 1;
    }

    *this->frameBase = this->interpolator->interpolate(
        this->keyFrames[startIndex], this->keyFrames[nextIndex],
        this->keyTimes[startIndex], this->keyTimes[nextIndex], diff);

    // auto slope = (this->keyFrames[nextIndex] - this->keyFrames[startIndex]) /
    //              (this->keyTimes[nextIndex] - this->keyTimes[startIndex]);
    // *this->frameBase = this->keyFrames[startIndex] +
    //                    (diff - this->keyTimes[startIndex]) * slope;

    /* todo - update frameBase with result from an interpolator object
            like this
            *this->frameBase = interpolator->interpolate(startKeyFrame,
       endKeyFrame, startTime, endTime, currTime);
    */
  }

  int getKeyFrameCount() { return this->keyFrames.size(); }

 private:
  int getCurrLowTimeIndex(float currTime) {
    // implicitly assumes keyTimes vector is sorted
    int index = -1;
    if (this->keyTimes[0] > currTime) {
      return index;
    }

    if (this->keyTimes[this->keyTimes.size() - 1] < currTime) {
      return this->keyTimes.size() - 1;
    }

    for (int i = 0; i < this->keyTimes.size() - 1; i++) {
      if (this->keyTimes[i] < currTime && this->keyTimes[i + 1] > currTime) {
        index = i;
        return index;
      }
    }
  }

  std::vector<T> keyFrames;
  std::vector<float> keyTimes;
  T *frameBase;

  std::chrono::time_point<std::chrono::system_clock> startTime;
  std::chrono::time_point<std::chrono::system_clock> currTime;

  std::unique_ptr<IInterpolator<T>> interpolator;
};
}

#endif  // COREENGINE_IANIMATION_H

/*

We want to animate the properties of a game object - position, rotation, color,
alpha etc.
We need to interpolate between a start and a stop animated property
For e.g.
    - animate position between (0, 0, 0) and (1, 1, 1) in 10 seconds
    - animate alpha between 0.0f and 1.0f in 2 seconds
    - keyframes
    - keyframe times
    - update method : should this be a fixed update or frame ny frame?
    - is looping animation?
    - start method
    - how to deduce time from current frame? getCurrentTime? we need to store
starting time then
    - interpolation algorithm? linear
update Method
    getCurrentTime
    getCurrentAnimValue

*/
