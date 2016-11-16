////
//// Created by chimeralabs on 8/8/2016.
////
//
//#include <android/log.h>
//#include <jni.h>
//#include <memory>
//
//#include <android/native_window.h> // requires ndk r5 or newer
//#include <android/native_window_jni.h> // requires ndk r5 or newer
//
//
//#include <hmdImplementations/RendererGearVR.h>
//#include <applications/include/Image360.h>
//
//using namespace CL;
//
//#define JNI_METHOD(return_type, method_name) \
//  JNIEXPORT return_type JNICALL              \
//      Java_co_chimeralabs_sdk_android_adapter_Image360JniAdapterGearVR_##method_name
//
//static ANativeWindow *window = 0;
//
//namespace {
//
//    inline jlong jptr(Image360 *image360) {
//        return reinterpret_cast<intptr_t>(image360);
//    }
//
//    inline Image360 *native(jlong ptr) {
//        return reinterpret_cast<Image360 *>(ptr);
//    }
//
//}  // anonymous namespace
//
//
//extern "C" {
//JNI_METHOD(jlong, createImage360NativeGearVR)(JNIEnv *env, jclass clazz,
//                                        jobject class_loader,
//                                        jobject android_context,
//                                        jlong native_image360gear_api){
//    return jptr(
//            new Image360(new RendererGearVR(env, class_loader))
//    );
//}
//
//JNI_METHOD(void, onSurfaceCreatedNativeGearVR) (JNIEnv *env, jobject obj,
//                                          jlong classHandle, jobject surface){
//    /*if (surface != 0) {
//        window = ANativeWindow_fromSurface(env, surface);
//        native(classHandle)->setWindow(window);
//    }*/
//}
//
//JNI_METHOD(void, onSurfaceChangedNativeGearVR) (JNIEnv *env, jobject obj,
//                                          jlong classHandle, jobject surface, jint format, jint width, jint height){
//    if (surface != 0) {
//        window = ANativeWindow_fromSurface(env, surface);
//        RendererGearVR* renderer = (RendererGearVR*)native(classHandle)->getRenderer();
//        renderer->setWindow(window);
//        EventQueue::getInstance()->push(Event(Event::START, NULL));
//    } else {
//        ANativeWindow_release(window);
//    }
//}
//
//JNI_METHOD(void, onSurfaceDestroyedNativeGearVR) (JNIEnv *env, jobject obj,
//                                            jlong classHandle, jobject surface){
//    ANativeWindow_release(window);
//    EventQueue::getInstance()->push(Event(Event::STOP, NULL));
//}
//
//JNI_METHOD(void, onResumeNativeGearVR) (JNIEnv *env, jobject, jlong classHandle){
//    native(classHandle)->startRenderThread();
//}
//
//JNI_METHOD(void, onPauseNativeGearVR) (JNIEnv *env, jobject, jlong classHandle){
//    native(classHandle)->stopRenderThread();
//}
//
//JNI_METHOD(void, onStopNativeGearVR) (JNIEnv *env, jobject, jlong classHandle){
//    Image360 *obj = native(classHandle);
//    delete obj;
//}
//
//}