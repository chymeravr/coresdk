#include <CloseMeListenerAndroidGvr.h>
#include <assert.h>

// a listener on the close button / box in the scene - we may want to animate this in future
namespace cl{
    CloseMeListenerAndroidGvr::CloseMeListenerAndroidGvr(ILoggerFactory* loggerFactory){
        assert(loggerFactory != nullptr);
        this->logger = loggerFactory->createLogger(tag);
    }
    void CloseMeListenerAndroidGvr::onGazeStarted(){
        this->focus = true;
    }
    void CloseMeListenerAndroidGvr::onGazeEnded(){
        this->focus = false;
    }
    void CloseMeListenerAndroidGvr::onGaze(){}
    
    bool CloseMeListenerAndroidGvr::inFocus(){
        return this->focus;
    }
}