#ifndef COREENGINE_ITIMEKEEPER_H
#define COREENGINE_ITIMEKEEPER_H

// warning : this class may not be thread safe ~ it will not return cpu time but
// wall clock time

namespace cl {
class ITimeKeeper {
 public:
  virtual ~ITimeKeeper() {}

  virtual void start() = 0;

  // returns the duration of time elapsed since start was called in seconds
  virtual float getElapsedTime() = 0;

  virtual void reset() = 0;
};
}

#endif  // COREENGINE_ITIMEKEEPER_H
