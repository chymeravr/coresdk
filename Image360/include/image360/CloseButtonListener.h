#ifndef IMAGE360_CLOSEBUTTONLISTENER_H
#define IMAGE360_CLOSEBUTTONLISTENER_H

#include <assert.h>
#include <coreEngine/components/gazeDetector/EventGazeListener.h>
#include <coreEngine/util/ILoggerFactory.h>

namespace cl {
class CloseButtonListener : public EventGazeListener {
 public:
  CloseButtonListener(ILoggerFactory *loggerFactory) {
    assert(loggerFactory != nullptr);
    this->logger = loggerFactory->createLogger(tag);
  }

  void setReticleEnterAnimation(Animation<CL_Vec4> &reticleEnterAnimation) {
	  this->reticleEnterAnimation = &reticleEnterAnimation;
  }

  void setReticleEnterResizeAnimation(Animation<CL_Vec3> &reticleEnterResizeAnimation) {
	  this->reticleEnterResizeAnimation = &reticleEnterResizeAnimation;
  }

  void setReticleLeaveAnimation(Animation<CL_Vec4> &reticeLeaveAnimation) {
	  this->reticleLeaveAnimation = &reticeLeaveAnimation;
  }

  void setReticleLeaveResizeAnimation(Animation<CL_Vec3> &reticleLeaveResizeAnimation) {
	  this->reticleLeaveResizeAnimation = &reticleLeaveResizeAnimation;
  }

  void onGazeStarted() {
    logger->log(LOG_INFO, "CloseMe: OnGazeStarted");
    this->focus = true;

	// start transition in animation
	if (this->reticleEnterAnimation != nullptr) {
		//this->logger->log(LOG_DEBUG, "Starting Action Button Animation");
		this->reticleEnterAnimation->start();
	}
	if (this->reticleEnterResizeAnimation != nullptr) {
		//this->logger->log(LOG_DEBUG, "Starting Resize Action Button Animation");
		this->reticleEnterResizeAnimation->start();
	}

	if (this->reticleLeaveAnimation != nullptr) {
		//this->logger->log(LOG_DEBUG, "Stopping Button Leave Animation");
		this->reticleLeaveAnimation->stop();
	}

	if (this->reticleLeaveResizeAnimation != nullptr) {
		this->logger->log(LOG_DEBUG, "Stopping Leave Resize Action Button Animation");
		this->reticleLeaveResizeAnimation->stop();
	}
  }

  void onGazeEnded() {
    logger->log(LOG_INFO, "CloseMe: OnGazeEnded");
    this->focus = false;

	if (this->reticleEnterAnimation != nullptr) {
		//this->logger->log(LOG_DEBUG, "Stopping Action Button Animation");
		this->reticleEnterAnimation->stop();
	}

	if (this->reticleEnterResizeAnimation != nullptr) {
		//this->logger->log(LOG_DEBUG, "Stopping Action Button Animation");
		this->reticleEnterResizeAnimation->stop();
	}

	// start transition out animation
	if (this->reticleLeaveAnimation != nullptr) {
		//this->logger->log(LOG_DEBUG, "Starting Button Leave Animation");
		this->reticleLeaveAnimation->start();
	}

	if (this->reticleLeaveResizeAnimation != nullptr) {
		this->logger->log(LOG_DEBUG, "Starting Leave Resize Action Button Animation");
		this->reticleLeaveResizeAnimation->start();
	}
  }

  void onGaze() {
    // logger->log(LOG_INFO, "CloseMe: OnGaze");
	  if (this->reticleEnterAnimation != nullptr) {
		  if (this->reticleEnterAnimation->isAnimationRunning()){
			  this->reticleEnterAnimation->update();
		  }
	  }

	  if (this->reticleEnterResizeAnimation != nullptr) {
		  if (this->reticleEnterResizeAnimation->isAnimationRunning()){
			  this->reticleEnterResizeAnimation->update();
		  }
	  }
  }

  bool inFocus() { return this->focus; }

 private:
  std::unique_ptr<ILogger> logger;
  std::string tag = "CloseButtonListener";
  bool focus = false;

  Animation<CL_Vec4> *reticleEnterAnimation = nullptr;
  Animation<CL_Vec3> *reticleEnterResizeAnimation = nullptr;

  Animation<CL_Vec4> *reticleLeaveAnimation = nullptr;
  Animation<CL_Vec3> *reticleLeaveResizeAnimation = nullptr;
};
}

#endif  // IMAGE360_CLOSEBUTTONLISTENER_H