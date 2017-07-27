//
// Created by robin_chimera on 7/26/2017.
//

#ifndef CVRANDROIDSDK_DAYDREAMACTIVITYCLOSEEVENT_H
#define CVRANDROIDSDK_DAYDREAMACTIVITYCLOSEEVENT_H

#include <jni.h>
#include <coreEngine/events/IEvent.h>

namespace cl{
    class DaydreamActivityCloseEvent : public IEvent {
    public:
        DaydreamActivityCloseEvent(JNIEnv *env, jobject *activitiyObject);
        void callListener();
    private:
        JNIEnv *env;
        jobject *activityObject;
    };
}

#endif //CVRANDROIDSDK_DAYDREAMACTIVITYCLOSEEVENT_H
