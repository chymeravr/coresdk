//
// Created by chimeralabs on 8/8/2016.
//

#include <android/log.h>
#include <jni.h>
#include <memory>

#include <android/native_window.h> // requires ndk r5 or newer
#include <android/native_window_jni.h> // requires ndk r5 or newer

#include <image360/Image360.h>
#include <hmdImplementations/RendererNoHMD.h>
#include <glImplementation/factory/FactoryPoolGL.h>
#include <androidImplementation/LoggerFactoryAndroidImpl.h>
#include <glImplementation/service/defaultImpl/ServiceGLPool.h>

using namespace cl;

#define JNI_METHOD(return_type, method_name) \
  JNIEXPORT return_type JNICALL              \
      Java_co_chimeralabs_sdk_android_adapter_Image360JniAdapterNoHMD_##method_name

static ANativeWindow *window = 0;

namespace {

    inline jlong jptr(Image360 *image360) {
        return reinterpret_cast<intptr_t>(image360);
    }

    inline Image360 *native(jlong ptr) {
        return reinterpret_cast<Image360 *>(ptr);
    }

}  // anonymous namespace


extern "C" {
JNI_METHOD(jlong, createImage360Native)(JNIEnv *env, jclass clazz,
                                  jobject class_loader,
                                  jobject android_context,
                                  jlong native_image360gear_api){
    LoggerFactory::setInstance(std::unique_ptr<LoggerFactory>(new LoggerFactoryAndroidImpl));
    IServicePool::setInstance(std::unique_ptr<IServicePool> (new ServiceGLPool));
    IServiceGLPool::setInstance(std::unique_ptr<IServiceGLPool>(new ServiceGLPool));
    return jptr(
            new Image360(std::unique_ptr<IRenderer>(new RendererNoHMD), std::unique_ptr<IFactoryPool> (new FactoryPoolGL))
    );
}

JNI_METHOD(void, onSurfaceCreatedNative) (JNIEnv *env, jobject obj,
                                            jlong classHandle, jobject surface){
    /*if (surface != 0) {
        window = ANativeWindow_fromSurface(env, surface);
        native(classHandle)->setWindow(window);
    }*/
}

JNI_METHOD(void, onSurfaceChangedNative) (JNIEnv *env, jobject obj,
                                            jlong classHandle, jobject surface, jint format, jint width, jint height){
    if (surface != 0) {
        window = ANativeWindow_fromSurface(env, surface);
        RendererNoHMD &renderer = (RendererNoHMD&)native(classHandle)->getRenderer();
        renderer.setWindow(window);
        EventQueue::getInstance()->push(Event(Event::START, NULL));
    } else {
        ANativeWindow_release(window);
    }
}

JNI_METHOD(void, onSurfaceDestroyedNative) (JNIEnv *env, jobject obj,
                                                jlong classHandle, jobject surface){
    ANativeWindow_release(window);
    EventQueue::getInstance()->push(Event(Event::STOP, NULL));
}

JNI_METHOD(void, onResumeNative) (JNIEnv *env, jobject, jlong classHandle){
    native(classHandle)->startRenderThread();
}

JNI_METHOD(void, onPauseNative) (JNIEnv *env, jobject, jlong classHandle){
    native(classHandle)->stopRenderThread();
}

JNI_METHOD(void, onStopNative) (JNIEnv *env, jobject, jlong classHandle){
    Image360 *obj = native(classHandle);
    delete obj;
}

}