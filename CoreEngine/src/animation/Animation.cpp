//#include <coreEngine/animation/Animation.h>
//
//namespace cl {
//
//template<class T> void Animation<T>::setKeyFrames(std::tuple<T, float> keyFrames...) {
//  va_list ap;
//  int j;
//  va_start(ap, keyFrames);
//  for (j = 0; j < count; j++) {
//    std::tuple<T, float> currTuple = va_arg(ap, std::tuple<T, float>);
//    this->keyFrames.push_back(std::get<0>(currTuple));
//
//    auto timePoint = std::get<1>(currTyple);
//    if (timePoint < 0.0f) {
//      throw std::invalid_argument("KeyFrame Time Must be Positive");
//    } else if (timePoint < this->keyTime.back()) {
//      throw std::invalid_argument(
//          "Current KeyFrame Time Must be Greater than previous KeyFrame Time");
//    }
//
//    this->keyTimes.push_back(timePoint);
//  }
//
//  va_end(ap);
//}
//
//// start the animations - set start time by fetching the system time
//template<class T> void Animation<T>::start() {
//  this->startTime = std::chrono::high_resolution_clock::now();
//  this->currTime = this->startTime;
//}
//
//template<class T> void Animation<T>::setFrameBase(T &frameBase) { this->frameBase = &frameBase; }
//
//template<class T> void Animation<T>::update() {
//  this->currTime = std::chrono::system_clock::now();
//  std::chrono::duration<float> cDiff = this->startTime - this->currTime;
//  auto diff = (float)cDiff.count();
//  // auto start_frame = ?
//  // auto end_frame = ?
//  // auto start_time = ?
//  // auto end_time = ?
//  // auto currTime = ?
//  // this->frameBase =
//  //     linearInterpolate(start_frame, end_frame, start_time, end_time,
//  //     currTime);
//}
//
//template<class T> T Animation<T>::getCurrLowFrame(float currTime) {
//  auto index = this->getCurrLowTimeIndex(currTime);
//  assert(index > -1 && index < this->keyTimes.size());
//
//  return this->keyFrames[index];
//}
//
//template<class T> int Animation<T>::getCurrLowTimeIndex(float currTime) {
//  assert(currTime > 0.0f);
//
//  // what happens if key frame starts from non zero time?
//
//  for (int i = 0; i < this->keyTimes.size() - 1; i++) {
//    if (this->keyTimes[i] > currTime){
//        // invalid input
//      return -1;
//    }
//    if(this->keyTimes[i+1] > currTime && this->keyTimes[i] < currTime){
//      return i;
//    }
//  }
//
//  return this->keyTimes.size() - 1;
//}
//
//
//}