//
// Created by robin_chimera on 7/26/2017.
//

#ifndef CVRANDROIDSDK_DAYDREAMACTIVITYCLOSEEVENT_H
#define CVRANDROIDSDK_DAYDREAMACTIVITYCLOSEEVENT_H

#include <jni.h>
#include <coreEngine/events/IEvent.h>
#include <coreEngine/util/ILoggerFactory.h>

namespace cl{
    class DaydreamActivityCloseEvent : public IEvent {
    public:
        DaydreamActivityCloseEvent(JNIEnv *env, jobject activitiyObject, ILoggerFactory& loggerFactory);
        void callListener();
        ~DaydreamActivityCloseEvent();
    private:
//        JNIEnv *env;
        JavaVM *jvm;
        jint version;
        jint attachStatus;
        jobject activityObject;
        std::unique_ptr<ILogger> logger = nullptr;
    };
}

#endif //CVRANDROIDSDK_DAYDREAMACTIVITYCLOSEEVENT_H
