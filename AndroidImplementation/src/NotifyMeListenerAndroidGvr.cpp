#include <NotifyMeListenerAndroidGvr.h>
#include <assert.h>

// a listener on the close button / box in the scene - we may want to animate this in future
namespace cl{
    NotifyMeListenerAndroidGvr::NotifyMeListenerAndroidGvr(ILoggerFactory* loggerFactory){
        assert(loggerFactory != nullptr);
        this->logger = loggerFactory->createLogger(tag);
    }
    void NotifyMeListenerAndroidGvr::onGazeStarted(){
        this->focus = true;
    }
    void NotifyMeListenerAndroidGvr::onGazeEnded(){
        this->focus = false;
    }
    void NotifyMeListenerAndroidGvr::onGaze(){}
    
    bool NotifyMeListenerAndroidGvr::inFocus(){
        return this->focus;
    }
}