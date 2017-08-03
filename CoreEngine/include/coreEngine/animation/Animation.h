#ifndef COREENGINE_ANIMATION_H
#define COREENGINE_ANIMATION_H

#include <assert.h>
#include <coreEngine/animation/IInterpolator.h>
#include <coreEngine/animation/ITimeKeeper.h>
#include <coreEngine/util/ILoggerFactory.h>
#include <iostream>
#include <memory>
#include <vector>

// todo - a reset method might be useful

namespace cl {
template <class T>
class Animation {
 public:
  Animation(ILoggerFactory &loggerFactory, T &frameBase,
            std::unique_ptr<IInterpolator<T>> interpolator,
            std::unique_ptr<ITimeKeeper> timeKeeper) {
    assert(interpolator != nullptr);
    assert(timeKeeper != nullptr);
    this->logger = loggerFactory.createLogger("Animation: ");
    this->frameBase = &frameBase;
    this->interpolator = std::move(interpolator);
    this->timeKeeper = std::move(timeKeeper);
  }

  ~Animation() {}

  void setKeyFrames(std::vector<T> keyFrames, std::vector<float> keyTimes) {
    // todo - error handling. invariant definitions blah blah
    assert(keyFrames.size() == keyTimes.size());
    // validate keyTimes sequence
    assert(isValidTimeSequence(keyTimes));

    for (int i = 0; i < keyFrames.size(); i++) {
      this->keyFrames.push_back(keyFrames[i]);
      this->keyTimes.push_back(keyTimes[i]);
    }
  }

  // start the animations - set start time by fetching the system time
  void start() {
    this->timeKeeper->start();
    this->isAnimationStarted = true;
  }

  void stop() {
    this->isAnimationStarted = false;
    this->isAnimationComplete = false;
    this->timeKeeper->reset();
    this->logger->log(LOG_DEBUG, "Stopping Animation");
  }

  void update() {
    auto elapsedTime = this->timeKeeper->getElapsedTime();
    auto startIndex = this->getCurrentLowTimeIndex(elapsedTime);
    assert(startIndex > -1 && startIndex < this->keyTimes.size());

    if (startIndex == this->keyTimes.size() - 1) {
      // we have reached the end of all keyframes
      *this->frameBase = this->keyFrames[startIndex];
      // this->isAnimationComplete = true;
      this->stop();
      return;
    }

    auto nextIndex = startIndex + 1;

    *this->frameBase = this->interpolator->interpolate(
        this->keyFrames[startIndex], this->keyFrames[nextIndex],
        this->keyTimes[startIndex], this->keyTimes[nextIndex], elapsedTime);
  }

  int getKeyFrameCount() { return this->keyFrames.size(); }

  bool isAnimationRunning() {
    return this->isAnimationStarted && !this->isAnimationComplete;
  }

  T getCurrentFrameValue() { return *frameBase; }

 private:
  int getCurrentLowTimeIndex(float currTime) {
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

    return 0;
  }

  bool isValidTimeSequence(std::vector<float> timePoints) {
    auto size = timePoints.size();
    auto currentTimePoint = timePoints[0];

    // we need non negative time points
    if (currentTimePoint < 0.0f) {
      return false;
    }

    // we need timePoints to be in increasing order
    for (int i = 1; i < size; i++) {
      if (currentTimePoint > timePoints[i]) {
        // decreasing subsequence detected - return false
        return false;
      }
      // increment and test next;
      currentTimePoint = timePoints[i];
    }
    return true;
  }

  std::vector<T> keyFrames;
  std::vector<float> keyTimes;
  T *frameBase;

  std::unique_ptr<ITimeKeeper> timeKeeper;

  std::unique_ptr<IInterpolator<T>> interpolator;

  std::unique_ptr<ILogger> logger;

  bool isAnimationStarted = false;
  bool isAnimationComplete = false;
};
}

#endif  // COREENGINE_IANIMATION_H