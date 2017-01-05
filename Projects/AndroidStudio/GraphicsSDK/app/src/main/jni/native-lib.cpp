#include <jni.h>
#include <string>

#include <android/log.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>

#include <memory>

#include <image360/Image360.h>
#include <LoggerAndroid.h>
#include <LoggerAndroidFactory.h>
#include <RendererGearVR.h>

#include <glImplementation/factory/ModelGLFactory.h>
#include <glImplementation/factory/SceneGLFactory.h>
#include <glImplementation/factory/CameraGLFactory.h>

#include <glImplementation/factory/opengles3/DiffuseTextureCubeMapGLES3Factory.h>
#include <glImplementation/factory/opengles3/DiffuseTextureGLES3Factory.h>

#include <coreEngine/components/transform/TransformCameraFactory.h>
#include <coreEngine/components/transform/TransformModelFactory.h>
#include <coreEngine/events/EventQueue.h>

#include <MutexLockAndroid.h>

//
using namespace cl;

#define JNI_METHOD(return_type, method_name) \
  JNIEXPORT return_type JNICALL              \
      Java_com_chymeravr_graphicssdk_GLES3JNILib_##method_name

static ANativeWindow *window = 0;
std::unique_ptr<Image360> application;
std::unique_ptr<ILogger> logger;
std::unique_ptr<IEventQueue> eventQueue = nullptr;
std::unique_ptr<LoggerFactoryAndroid> loggerFactory(new LoggerFactoryAndroid());

namespace {

    inline jlong jptr(Image360 *image360) {
        return reinterpret_cast<intptr_t>(image360);
    }

    inline Image360 *native(jlong ptr) {
        return reinterpret_cast<Image360 *>(ptr);
    }

}  // anonymous namespace

//
//extern "C" {
//JNI_METHOD(jlong, onCreate)(JNIEnv *env, jobject obj, jobject activity){
//    logger = loggerFactory->createLogger("TestAppAndroidauncher: ");
//    logger->log(LOG_DEBUG, "Testing logger.");
//    //std::unique_ptr<cl::SceneGLFactory> sceneFactory(new cl::SceneGLFactory(loggerFactory.get()));
//    std::unique_ptr<ISceneFactory> sceneFactory(new SceneGLFactory(loggerFactory.get()));
//    std::unique_ptr<IModelFactory> modelFactory(new ModelGLFactory(loggerFactory.get()));
//    std::unique_ptr<IDiffuseTextureFactory> diffuseTextureFactory(new DiffuseTextureGLES3Factory(loggerFactory.get()));
//    std::unique_ptr<IDiffuseTextureCubeMapFactory> diffuseTextureCubeMapFactory(new DiffuseTextureCubeMapGLES3Factory(loggerFactory.get()));
//    std::unique_ptr<IRenderer> renderer(new RendererGearVR(env, activity));
//    std::unique_ptr<TransformCameraFactory> transformCameraFactory(new TransformCameraFactory(loggerFactory.get()));
//    std::unique_ptr<TransformModelFactory> transformModelFactory(new TransformModelFactory(loggerFactory.get()));
//    std::unique_ptr<ICameraFactory> cameraFactory(new CameraGLFactory(loggerFactory.get()));
//    std::unique_ptr<IMutexLock> mutexLock(new MutexLockAndroid());
//    eventQueue = std::unique_ptr<EventQueue>(new EventQueue(std::move(mutexLock)));
//
//    return jptr(
//            new Image360(std::move(renderer),
//                         std::move(sceneFactory),
//                         std::move(modelFactory),
//                         nullptr,
//                         nullptr,
//                         std::move(transformCameraFactory),
//                         std::move(transformModelFactory),
//                         std::move(cameraFactory),
//                         eventQueue.get(),
//                         loggerFactory.get())
//    );
//}
//
//JNI_METHOD(void, onSurfaceCreated) (JNIEnv *env, jobject obj,
//                                                jlong classHandle, jobject surface){
////    if (surface != 0) {
////        window = ANativeWindow_fromSurface(env, surface);
////        native(classHandle)->setWindow(window);
////    }
//}
//
//JNI_METHOD(void, onSurfaceChanged) (JNIEnv *env, jobject obj,
//                                                jlong classHandle, jobject surface, jint format, jint width, jint height){
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
//JNI_METHOD(void, onSurfaceDestroyed) (JNIEnv *env, jobject obj,
//                                                  jlong classHandle, jobject surface){
//    ANativeWindow_release(window);
//    EventQueue::getInstance()->push(Event(Event::STOP, NULL));
//}
//
//JNI_METHOD(void, onResume) (JNIEnv *env, jobject, jlong classHandle){
//    native(classHandle)->startRenderThread();
//}
//
//JNI_METHOD(void, onPause) (JNIEnv *env, jobject, jlong classHandle){
//    native(classHandle)->stopRenderThread();
//}
//
//JNI_METHOD(void, onStop) (JNIEnv *env, jobject, jlong classHandle){
//    Image360 *obj = native(classHandle);
//    delete obj;
//}
//
//}

/*
================================================================================

Activity lifecycle

================================================================================
*/

JNIEXPORT jstring JNICALL
Java_com_chymeravr_graphicssdk_GLES3JNILib_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

JNI_METHOD(jlong, onCreate)( JNIEnv * env, jobject activity )
{
    //ALOGV( "    GLES3JNILib::onCreate()" );
    logger = loggerFactory->createLogger("Image360_Android_Test");
    logger->log(LOG_DEBUG, "GLES3JNILib::onCreate()");
    RendererGearVR *renderer = new RendererGearVR(env, activity);
//    ovrAppThread_Create( appThread, env, activity );
    //renderer->start();

//    ovrMessageQueue_Enable( &appThread->MessageQueue, true );
//    ovrMessage message;
//    ovrMessage_Init( &message, MESSAGE_ON_CREATE, MQ_WAIT_PROCESSED );
//    ovrMessageQueue_PostMessage( &appThread->MessageQueue, &message );


    //EventQueue

    return (jlong)((size_t)renderer);
}

JNI_METHOD(void, onStart)( JNIEnv * env, jobject obj, jlong handle )
{
    logger->log(LOG_DEBUG,  "    GLES3JNILib::onStart()" );
    //ovrAppThread * appThread = (ovrAppThread *)((size_t)handle);
//    ovrMessage message;
//    ovrMessage_Init( &message, MESSAGE_ON_START, MQ_WAIT_PROCESSED );
//    ovrMessageQueue_PostMessage( &appThread->MessageQueue, &message );
    RendererGearVR *renderer = (RendererGearVR *)((size_t)handle);
    renderer->start();
}

JNI_METHOD(void, onResume)( JNIEnv * env, jobject obj, jlong handle )
{
    logger->log(LOG_DEBUG, "    GLES3JNILib::onResume()" );
//    ovrAppThread * appThread = (ovrAppThread *)((size_t)handle);
//    ovrMessage message;
//    ovrMessage_Init( &message, MESSAGE_ON_RESUME, MQ_WAIT_PROCESSED );
//    ovrMessageQueue_PostMessage( &appThread->MessageQueue, &message );

}

JNI_METHOD(void, onPause)( JNIEnv * env, jobject obj, jlong handle )
{
    logger->log(LOG_DEBUG, "    GLES3JNILib::onPause()" );
//    ovrAppThread * appThread = (ovrAppThread *)((size_t)handle);
//    ovrMessage message;
//    ovrMessage_Init( &message, MESSAGE_ON_PAUSE, MQ_WAIT_PROCESSED );
//    ovrMessageQueue_PostMessage( &appThread->MessageQueue, &message );
}

JNI_METHOD(void, onStop)( JNIEnv * env, jobject obj, jlong handle )
{
    logger->log(LOG_DEBUG, "    GLES3JNILib::onStop()" );
//    ovrAppThread * appThread = (ovrAppThread *)((size_t)handle);
//    ovrMessage message;
//    ovrMessage_Init( &message, MESSAGE_ON_STOP, MQ_WAIT_PROCESSED );
//    ovrMessageQueue_PostMessage( &appThread->MessageQueue, &message );
}

JNI_METHOD(void, onDestroy)( JNIEnv * env, jobject obj, jlong handle )
{
    logger->log(LOG_DEBUG, "    GLES3JNILib::onDestroy()" );
//    ovrAppThread * appThread = (ovrAppThread *)((size_t)handle);
//    ovrMessage message;
//    ovrMessage_Init( &message, MESSAGE_ON_DESTROY, MQ_WAIT_PROCESSED );
//    ovrMessageQueue_PostMessage( &appThread->MessageQueue, &message );
//    ovrMessageQueue_Enable( &appThread->MessageQueue, false );
//
//    ovrAppThread_Destroy( appThread, env );
    RendererGearVR *renderer = (RendererGearVR *)((size_t)handle);
    renderer->destroy();
    //free( appThread );
}

/*
================================================================================

Surface lifecycle

================================================================================
*/

JNI_METHOD(void, onSurfaceCreated)( JNIEnv * env, jobject obj, jlong handle, jobject surface )
{
    logger->log(LOG_DEBUG,  "    GLES3JNILib::onSurfaceCreated()" );
    //ovrAppThread * appThread = (ovrAppThread *)((size_t)handle);
    RendererGearVR *renderer = (RendererGearVR *)((size_t)handle);

    ANativeWindow * newNativeWindow = ANativeWindow_fromSurface( env, surface );
    if ( ANativeWindow_getWidth( newNativeWindow ) < ANativeWindow_getHeight( newNativeWindow ) )
    {
        // An app that is relaunched after pressing the home button gets an initial surface with
        // the wrong orientation even though android:screenOrientation="landscape" is set in the
        // manifest. The choreographer callback will also never be called for this surface because
        // the surface is immediately replaced with a new surface with the correct orientation.
        logger->log(LOG_DEBUG, "        Surface not in landscape mode!" );
    }

    logger->log(LOG_DEBUG, "        NativeWindow = ANativeWindow_fromSurface( env, surface )" );
//    appThread->NativeWindow = newNativeWindow;
//    ovrMessage message;
//    ovrMessage_Init( &message, MESSAGE_ON_SURFACE_CREATED, MQ_WAIT_PROCESSED );
//    ovrMessage_SetPointerParm( &message, 0, appThread->NativeWindow );
//    ovrMessageQueue_PostMessage( &appThread->MessageQueue, &message );
    renderer->setWindow(newNativeWindow);
}

JNI_METHOD(void, onSurfaceChanged)( JNIEnv * env, jobject obj, jlong handle, jobject surface )
{
    logger->log(LOG_DEBUG, "    GLES3JNILib::onSurfaceChanged()" );
//    ovrAppThread * appThread = (ovrAppThread *)((size_t)handle);
    RendererGearVR *renderer = (RendererGearVR*)((size_t)handle);

    ANativeWindow * newNativeWindow = ANativeWindow_fromSurface( env, surface );
    if ( ANativeWindow_getWidth( newNativeWindow ) < ANativeWindow_getHeight( newNativeWindow ) )
    {
        // An app that is relaunched after pressing the home button gets an initial surface with
        // the wrong orientation even though android:screenOrientation="landscape" is set in the
        // manifest. The choreographer callback will also never be called for this surface because
        // the surface is immediately replaced with a new surface with the correct orientation.
        logger->log(LOG_DEBUG, "        Surface not in landscape mode!" );
    }

    if ( newNativeWindow != renderer->getWindow() )
    {
        if ( renderer->getWindow() != nullptr )
        {
//            ovrMessage message;
//            ovrMessage_Init( &message, MESSAGE_ON_SURFACE_DESTROYED, MQ_WAIT_PROCESSED );
//            ovrMessageQueue_PostMessage( &appThread->MessageQueue, &message );
            logger->log(LOG_DEBUG, "        ANativeWindow_release( NativeWindow )" );
            ANativeWindow_release( renderer->getWindow() );
            renderer->setWindow(nullptr);
        }
        if ( newNativeWindow != nullptr )
        {
            logger->log(LOG_DEBUG, "        NativeWindow = ANativeWindow_fromSurface( env, surface )" );
            renderer->setWindow(newNativeWindow);
//            ovrMessage message;
//            ovrMessage_Init( &message, MESSAGE_ON_SURFACE_CREATED, MQ_WAIT_PROCESSED );
//            ovrMessage_SetPointerParm( &message, 0, appThread->NativeWindow );
//            ovrMessageQueue_PostMessage( &appThread->MessageQueue, &message );
        }
    }
    else if ( newNativeWindow != nullptr )
    {
        ANativeWindow_release( newNativeWindow );
    }
}

JNI_METHOD(void, onSurfaceDestroyed)( JNIEnv * env, jobject obj, jlong handle )
{
    logger->log(LOG_DEBUG, "    GLES3JNILib::onSurfaceDestroyed()" );
    RendererGearVR *renderer = (RendererGearVR*)((size_t)handle);
//    ovrMessage message;
//    ovrMessage_Init( &message, MESSAGE_ON_SURFACE_DESTROYED, MQ_WAIT_PROCESSED );
//    ovrMessageQueue_PostMessage( &appThread->MessageQueue, &message );
    logger->log(LOG_DEBUG, "        ANativeWindow_release( NativeWindow )" );
    ANativeWindow_release( renderer->getWindow() );
    renderer->setWindow(nullptr);
}

/*
================================================================================

Input

================================================================================
*/

JNI_METHOD(void, onKeyEvent)( JNIEnv * env, jobject obj, jlong handle, int keyCode, int action )
{
//    if ( action == AKEY_EVENT_ACTION_UP )
//    {
        logger->log(LOG_DEBUG, "    GLES3JNILib::onKeyEvent( %d, %d )" );
//    }
//    ovrAppThread * appThread = (ovrAppThread *)((size_t)handle);
//    ovrMessage message;
//    ovrMessage_Init( &message, MESSAGE_ON_KEY_EVENT, MQ_WAIT_NONE );
//    ovrMessage_SetIntegerParm( &message, 0, keyCode );
//    ovrMessage_SetIntegerParm( &message, 1, action );
//    ovrMessageQueue_PostMessage( &appThread->MessageQueue, &message );
}

JNI_METHOD(void, onTouchEvent)( JNIEnv * env, jobject obj, jlong handle, int action, float x, float y )
{
//    if ( action == AMOTION_EVENT_ACTION_UP )
//    {
        logger->log(LOG_DEBUG, "    GLES3JNILib::onTouchEvent( %d, %1.0f, %1.0f )");
//    }
//    ovrAppThread * appThread = (ovrAppThread *)((size_t)handle);
//    ovrMessage message;
//    ovrMessage_Init( &message, MESSAGE_ON_TOUCH_EVENT, MQ_WAIT_NONE );
//    ovrMessage_SetIntegerParm( &message, 0, action );
//    ovrMessage_SetFloatParm( &message, 1, x );
//    ovrMessage_SetFloatParm( &message, 2, y );
//    ovrMessageQueue_PostMessage( &appThread->MessageQueue, &message );
}
