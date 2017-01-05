#include <jni.h>
#include "glesNative.h"



#ifdef __cplusplus
extern "C" {
#endif

GLESNative *gGlesObject;
cl::LoggerFactoryAndroid* loggerFactory;


JNIEXPORT void JNICALL
Java_com_chymeravr_testgraphicssdk_GLESContextActivity_CreateObjectNative(JNIEnv *env,
                                                                           jobject instance, jobject assetManager) {
    std::string tag = "GLESNative";
    loggerFactory = new cl::LoggerFactoryAndroid();

    AAssetManager* mgr = AAssetManager_fromJava(env, assetManager);
    gGlesObject = new GLESNative(loggerFactory, mgr);


}

JNIEXPORT void JNICALL
Java_com_chymeravr_testgraphicssdk_GLESContextActivity_DeleteObjectNative(JNIEnv *env,
                                                                                   jobject instance) {
    if (gGlesObject != nullptr) {
        delete gGlesObject;
    }
    if (loggerFactory != nullptr)
    {
        delete loggerFactory;
    }
    gGlesObject = nullptr;
    loggerFactory = nullptr;
}

JNIEXPORT jboolean JNICALL
Java_com_chymeravr_testgraphicssdk_GLESContextActivity_IsInitsDoneNative(JNIEnv *env,
                                                                                  jobject instance) {

    if (gGlesObject == NULL || !gGlesObject->IsInitsDone()) {
        return JNI_FALSE;
    } else {
        return JNI_TRUE;
    }

}


JNIEXPORT jstring JNICALL
Java_com_chymeravr_testgraphicssdk_GLESContextActivity_GetGLESVersionNative(JNIEnv *env,
                                                                                     jobject instance) {

    std::string retValue = "";
    if (gGlesObject == NULL) {
        return env->NewStringUTF(retValue.c_str());
    }
    retValue = gGlesObject->GetGLESVersionInfo();
    return env->NewStringUTF(retValue.c_str());
}

#ifdef __cplusplus
}
#endif
