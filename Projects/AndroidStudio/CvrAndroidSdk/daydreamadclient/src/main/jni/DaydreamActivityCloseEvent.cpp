//
// Created by robin_chimera on 7/26/2017.
//

#include <assert.h>
#include "DaydreamActivityCloseEvent.h"

namespace cl{
    DaydreamActivityCloseEvent::DaydreamActivityCloseEvent(JNIEnv *env, jobject activitiyObject, ILoggerFactory& loggerFactory){
        //this->env = env;
        env->GetJavaVM(&this->jvm);
        this->version = env->GetVersion();
        this->activityObject = env->NewGlobalRef(activitiyObject);
        this->logger = loggerFactory.createLogger("DaydreamActivityCloseEvent");
    }

    void DaydreamActivityCloseEvent::callListener(){
        this->logger->log(LOG_DEBUG, "Daydream Activity Close Listener Called");
        // First get the class that contains the method you need to call
//        jclass clazz = env->FindClass("com.chymeravr.daydreamadclient.Image360Activity");
        JNIEnv *env;

        JavaVMAttachArgs args;
        args.version = this->version; // choose your JNI version
        args.name = NULL; // you might want to give the java thread a name
        args.group = NULL; // you might want to assign the java thread to a ThreadGroup
        this->attachStatus = jvm->AttachCurrentThread(&env, &args);

        if(attachStatus != JNI_OK){
            return;
        }
        // todo - null pointer checks
        jclass clazz = env->FindClass("com/chymeravr/daydreamadclient/Image360Activity");
        //jclass clazz = env->GetObjectClass(*this->activityObject);
        assert(clazz != nullptr);
        // Get the method that you want to call
        jmethodID closeActivity = env->GetMethodID(clazz, "finishAdActivity", "()V");
        assert(closeActivity != nullptr);
        // Call the method on the object
        env->CallVoidMethod(activityObject, closeActivity);

        env->DeleteGlobalRef(activityObject);

        this->logger->log(LOG_DEBUG, "Close Activity Complete");
    }
    DaydreamActivityCloseEvent::~DaydreamActivityCloseEvent() {
        this->logger->log(LOG_DEBUG, "Destructor Call for Daydream Activityi Close Event");
        if(this->attachStatus == JNI_OK) {
//            this->jvm->DetachCurrentThread();
        }
    }
}