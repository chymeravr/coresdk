//
// Created by robin_chimera on 3/22/2017.
// wires the ad application for daydream headset
//


#include <android/log.h>
#include <jni.h>
#include <memory>

#include "Image360AppWrapper.h"

#define JNI_METHOD(return_type, method_name) \
  JNIEXPORT return_type JNICALL              \
      Java_com_chymeravr_daydreamadclient_Image360Activity_##method_name

namespace {

    typedef enum{
        NO_EVENT=0,
        DOWNLOAD=1,
        CLOSE_AD=2
    } keyEventResponse;

    inline jlong jptr(cl::Image360AppWrapper *nativeImage360) {
        return reinterpret_cast<intptr_t>(nativeImage360);
    }

    inline cl::Image360AppWrapper *native(jlong ptr) {
        return reinterpret_cast<cl::Image360AppWrapper *>(ptr);
    }


}  // anonymous namespace

#ifdef __cplusplus
extern "C" {
#endif

JNI_METHOD(jlong, nativeCreateRenderer)
(JNIEnv *env, jclass clazz, jobject image360_activity, jobject android_context,
 jlong native_gvr_api, jstring appDirectory, jstring image360AdFileName,
 jstring controllerModelFilename, jstring controllerTextureFilename,
 jstring laserBeamTextureFilename) {

    auto appDir = env->GetStringUTFChars(appDirectory, JNI_FALSE);
    auto image360File = env->GetStringUTFChars(image360AdFileName, JNI_FALSE);
    auto controllerModelFile = env->GetStringUTFChars(controllerModelFilename, JNI_FALSE);
    auto controllerTextureFile = env->GetStringUTFChars(controllerTextureFilename, JNI_FALSE);
    auto laserBeamTextureFile = env->GetStringUTFChars(laserBeamTextureFilename, JNI_FALSE);

    return jptr( new cl::Image360AppWrapper(env, image360_activity, android_context, native_gvr_api,
                                            appDir, image360File, controllerModelFile,
                                            controllerTextureFile, laserBeamTextureFile));
}

JNI_METHOD(void, nativeOnStart)
(JNIEnv *env, jobject obj, jlong nativeImage360AppWrapper) {
    auto image360AppWrapper = native(nativeImage360AppWrapper);
    image360AppWrapper->onStart();
}

JNI_METHOD(void, nativeInitializeGl)
(JNIEnv *env, jobject obj, jlong nativeImage360AppWrapper) {
    auto image360AppWrapper = native(nativeImage360AppWrapper);
    image360AppWrapper->initialize();
}

JNI_METHOD(void, nativeDrawFrame)
(JNIEnv *env, jobject obj, jlong nativeImage360AppWrapper) {
    auto image360AppWrapper = native(nativeImage360AppWrapper);
    image360AppWrapper->draw();
}


JNI_METHOD(void, nativeOnPause)
(JNIEnv *env, jobject obj, jlong nativeImage360AppWrapper) {
    auto image360AppWrapper = native(nativeImage360AppWrapper);
    image360AppWrapper->onPause();
}

JNI_METHOD(void, nativeOnResume)
(JNIEnv *env, jobject obj, jlong nativeImage360AppWrapper) {
    auto image360AppWrapper = native(nativeImage360AppWrapper);
    image360AppWrapper->onResume();
}

JNI_METHOD(jfloatArray , nativeGetHmdParams)
(JNIEnv *env, jobject obj, jlong nativeImage360AppWrapper){
    // native function that returns quaternion representing current head rotation
    jfloatArray result;
    result = env->NewFloatArray(4);

    auto image360AppWrapper = native(nativeImage360AppWrapper);

    //todo  : modify renderer to include hmd parameters fetcher as a virtual function
    auto renderer = (cl::RendererDaydream *)image360AppWrapper->image360->getRenderer();

    auto hmdParams = renderer->getHMDParams();

    float params[4];
    for (int j = 0; j < 4; j++) {
        params[j] = hmdParams[j];
    }


    env->SetFloatArrayRegion(result, 0, 4, params);
    return result;
}

JNI_METHOD(void, nativeDestroyRenderer)
(JNIEnv *env, jclass clazz, jlong nativeImage360AppWrapper) {
    auto image360AppWrapper = native(nativeImage360AppWrapper);
    image360AppWrapper->onDestroy();
    delete image360AppWrapper;
}

#ifdef __cplusplus
}
#endif  // extern "C"

