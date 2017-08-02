#ifndef COREENGINE_CHRONOTIMEKEEPER_H
#define COREENGINE_CHRONOTIMEKEEPER_H

#include <coreEngine/animation/ITimeKeeper.h>
#include <chrono>

namespace cl {
class ChronoTimeKeeper : public ITimeKeeper {
 public:
  ~ChronoTimeKeeper() {}

  void start() {
	this->isStarted = true;
    this->startTime = std::chrono::high_resolution_clock::now();
    // this->currTime = this->startTime;
  }

  // returns the duration of time elapsed since start was called in seconds
  float getElapsedTime() {
    if (!this->isStarted) {
      return 0.0f;
    }
    this->currTime = std::chrono::system_clock::now();
    std::chrono::duration<float> cDiff = this->currTime - this->startTime;
    auto diff = (float)cDiff.count();
    return diff;
  }

 private:
  //? should these variables be deinitialized??
  std::chrono::time_point<std::chrono::system_clock> startTime;
  std::chrono::time_point<std::chrono::system_clock> currTime;

  bool isStarted = false;
};
}

#endif  // COREENGINE_CHRONOTIMEKEEPER_H
