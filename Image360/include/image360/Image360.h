#ifndef IMAGE360_IMAGE360_H
#define IMAGE360_IMAGE360_H

#include<memory>
#include<coreEngine/IRenderer.h>
#include<coreEngine/events/AppEventListener.h>

namespace cl{
    class Image360 : public AppEventListener{
    public:
        Image360(std::unique_ptr<IRenderer> renderer);

        //AppEventListener implementation
        void onAppStartEvent();
        void onAppPauseEvent();
        void onAppResumeEvent();
        void onAppStopEvent();
    private:
        std::unique_ptr<IRenderer> renderer;
    };
}

#endif //IMAGE360_IMAGE360_H