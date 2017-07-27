//
// Created by robin_chimera on 7/26/2017.
//

#include "DaydreamActivityCloseEvent.h"

namespace cl{
    DaydreamActivityCloseEvent::DaydreamActivityCloseEvent(JNIEnv *env, jobject *activitiyObject){
        this->env = env;
        this->activityObject = activitiyObject;
    }

    void DaydreamActivityCloseEvent::callListener(){
        // First get the class that contains the method you need to call
        jclass clazz = env->FindClass("com.chymeravr.daydreamadclient.Image360Activity");
        // Get the method that you want to call
        jmethodID closeActivity = env->GetMethodID(clazz, "finishActivity", "(V)V");
        // Call the method on the object
        env->CallVoidMethod(*activityObject, closeActivity);
    }
}