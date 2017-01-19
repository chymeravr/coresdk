/**
 * Native Activity lifecyle callbacks
 */

#include <jni.h>
#include <image360/Image360.h>
#include <LoggerAndroidFactory.h>

#include <RendererGearVR.h>
#include <glImplementation/factory/SceneGLFactory.h>

#include <MutexLockAndroid.h>
#include <glImplementation/factory/ModelGLFactory.h>
#include <glImplementation/factory/opengles3/DiffuseTextureGLES3Factory.h>
#include <glImplementation/factory/opengles3/DiffuseTextureCubeMapGLES3Factory.h>
#include <coreEngine/components/transform/TransformCameraFactory.h>
#include <coreEngine/components/transform/TransformModelFactory.h>
#include <coreEngine/events/EventQueue.h>
#include <ImageBMPLoaderAndroid.h>


#include <CameraGLOVRFactory.h>

#include <android/input.h>

#include <VrApi_SystemUtils.h>
#ifdef __cplusplus
extern "C" {
#endif

using namespace cl;



std::unique_ptr<LoggerFactoryAndroid> loggerFactory = nullptr;
std::unique_ptr<ILogger> logger = nullptr;
//AAssetManager *mgr = nullptr;
std::unique_ptr<IEventQueue> eventQueue = nullptr;
//pthread_t renderingThread;
//static volatile bool isEnabled = false;

/*
================================================================================

ovrMessageQueue

================================================================================
*/

typedef enum
{
    MQ_WAIT_NONE,       // don't wait
    MQ_WAIT_RECEIVED,   // wait until the consumer thread has received the message
    MQ_WAIT_PROCESSED   // wait until the consumer thread has processed the message
} ovrMQWait;


#define MAX_MESSAGE_PARMS	8                                                                       // max number of parameters in a message
#define MAX_MESSAGES		1024                                                                    // max message queue size

typedef struct
{
    int         Id;
    ovrMQWait   Wait;
    long long   Parms[MAX_MESSAGE_PARMS];
} ovrMessage;

static void ovrMessage_Init( ovrMessage * message, const int id, const ovrMQWait wait )
{
    message->Id = id;
    message->Wait = wait;
    memset( message->Parms, 0, sizeof( message->Parms ) );
}

static void		ovrMessage_SetPointerParm( ovrMessage * message, int index, void * ptr ) { *(void **)&message->Parms[index] = ptr; }
static void *	ovrMessage_GetPointerParm( ovrMessage * message, int index ) { return *(void **)&message->Parms[index]; }
static void		ovrMessage_SetIntegerParm( ovrMessage * message, int index, int value ) { message->Parms[index] = value; }
static int		ovrMessage_GetIntegerParm( ovrMessage * message, int index ) { return (int)message->Parms[index]; }
static void		ovrMessage_SetFloatParm( ovrMessage * message, int index, float value ) { *(float *)&message->Parms[index] = value; }
static float	ovrMessage_GetFloatParm( ovrMessage * message, int index ) { return *(float *)&message->Parms[index]; }


// Cyclic queue with messages.
typedef struct
{
    ovrMessage          Messages[MAX_MESSAGES];
    volatile int        Head;                           // dequeue at the head
    volatile int        Tail;                           // enqueue at the tail
    ovrMQWait           Wait;                           // i guess this is the wiaitng strategy - lets see the implementation to find out
    volatile bool       EnabledFlag;
    volatile bool       PostedFlag;
    volatile bool       ReceivedFlag;
    volatile bool       ProcessedFlag;
    pthread_mutex_t     Mutex;                         // a mutex provides a secure lock on shared data - synchronization becomes possible
    pthread_cond_t      PostedCondition;               // a condition (short for "conditional variable" ) is a synch device that allows threads to suspend execution and relinquish processor until some predicate
    // on shared data is satisfied. Basic ops - signal the condition - wait for a the condition
    pthread_cond_t      ReceivedCondition;
    pthread_cond_t      ProcessedCondition;
} ovrMessageQueue;

static void ovrMessageQueue_Create( ovrMessageQueue * messageQueue )
{
    messageQueue->Head = 0;
    messageQueue->Tail = 0;                             // queue starts and ends at 0 index of array
    messageQueue->Wait = MQ_WAIT_NONE;                  // none waiting strategy
    messageQueue->EnabledFlag = false;
    messageQueue->PostedFlag = false;
    messageQueue->ReceivedFlag = false;
    messageQueue->ProcessedFlag = false;

    pthread_mutexattr_t attr;
    pthread_mutexattr_init( &attr) ;
    pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_ERRORCHECK );                                   // This type of mutex provides error checking. A thread attempting to relock this mutex without first unlocking it
    // shall return with an error - basically don't let thread fuck up
    pthread_mutex_init( &messageQueue->Mutex, &attr );
    pthread_cond_init ( &messageQueue->PostedCondition, NULL);
    pthread_cond_init ( &messageQueue->ReceivedCondition, NULL);
    pthread_cond_init ( &messageQueue->ProcessedCondition, NULL);
}

static void ovrMessageQueue_Destroy( ovrMessageQueue * messageQueue )
{
    pthread_mutex_destroy( &messageQueue->Mutex );
    pthread_cond_destroy( &messageQueue->PostedCondition );
    pthread_cond_destroy( &messageQueue->ReceivedCondition );
    pthread_cond_destroy( &messageQueue->ProcessedCondition );
}

static void ovrMessageQueue_Enable( ovrMessageQueue * messageQueue, const bool set )
{
    messageQueue->EnabledFlag = set;
}

static void ovrMessageQueue_PostMessage( ovrMessageQueue * messageQueue, const ovrMessage * message )
{
    if ( !messageQueue->EnabledFlag )
    {
        return;
    }
    while ( messageQueue->Tail - messageQueue->Head >= MAX_MESSAGES )                               // line is full - wait for a while - you may sleep sir
    {
        usleep( 1000 );                                                                             // this could cause starvation
    }
    pthread_mutex_lock( &messageQueue->Mutex );                                                     // acquire lock
    messageQueue->Messages[messageQueue->Tail & ( MAX_MESSAGES - 1 )] = *message;                   // add message to the tail
    messageQueue->Tail++;
    messageQueue->PostedFlag = true;                                                                // set posted flag to true - check when this becomes false
    pthread_cond_broadcast( &messageQueue->PostedCondition );                                       // broadcast to all who wait on Posted condition that a message has been posted and is now available
    if ( message->Wait == MQ_WAIT_RECEIVED )                                                        // if message says to wait till received
    {
        while ( !messageQueue->ReceivedFlag )                                                       // if receivedflag is false - you sleep
        {
            pthread_cond_wait( &messageQueue->ReceivedCondition, &messageQueue->Mutex );            // the message queue has just issued a received flag condition. the post message method will block/sleep
            // it waits till the messageQueue issues a received condtion - possibly helps prevent build of too many messages
        }
        messageQueue->ReceivedFlag = false;                                                         // immediately set received to false - possibly for the next message to test on
    }
    else if ( message->Wait == MQ_WAIT_PROCESSED )                                                  // if message says to wait till it has been processed
    {
        while ( !messageQueue->ProcessedFlag )                                                      // processed flag is false
        {
            pthread_cond_wait( &messageQueue->ProcessedCondition, &messageQueue->Mutex );           // go to sleep since the queue is currently processing some other messagers
        }
        messageQueue->ProcessedFlag = false;                                                        // since the thread woke up, the queue must have processed everything and set the flag to true - time to make
    }                                                                                               // the bugger work
    pthread_mutex_unlock( &messageQueue->Mutex );
}

static void ovrMessageQueue_SleepUntilMessage( ovrMessageQueue * messageQueue )
{
    if ( messageQueue->Wait == MQ_WAIT_PROCESSED )                                                  // wait no longer my friends - the queue is empty since no more messages
    {                                                                                               // somebody must have set me to wait till processed- i shall wait no more
        messageQueue->ProcessedFlag = true;
        pthread_cond_broadcast( &messageQueue->ProcessedCondition );
        messageQueue->Wait = MQ_WAIT_NONE;
    }
    pthread_mutex_lock( &messageQueue->Mutex );
    if ( messageQueue->Tail > messageQueue->Head )                                                  // can't sleep - more stuff to do
    {
        pthread_mutex_unlock( &messageQueue->Mutex );
        return;
    }
    while ( !messageQueue->PostedFlag )                                                             // no messages have left in queue - I am going to sleep for a bit
    {
        pthread_cond_wait( &messageQueue->PostedCondition, &messageQueue->Mutex );                  // if someone posts a message - wake me up - i have a job to do
    }
    messageQueue->PostedFlag = false;
    pthread_mutex_unlock( &messageQueue->Mutex );

}

static bool ovrMessageQueue_GetNextMessage( ovrMessageQueue * messageQueue, ovrMessage * message, bool waitForMessages )
{
    if ( messageQueue->Wait == MQ_WAIT_PROCESSED )                                                  // wait no longer my friends i have processed a message on the queue
    {
        messageQueue->ProcessedFlag = true;
        pthread_cond_broadcast( &messageQueue->ProcessedCondition );
        messageQueue->Wait = MQ_WAIT_NONE;
    }
    if ( waitForMessages )
    {
        ovrMessageQueue_SleepUntilMessage( messageQueue );                                          // if message queue is empty - put me to sleep
    }
    pthread_mutex_lock( &messageQueue->Mutex );                                                     // else acquire lock and remove a message from the ehad of the queue
    if ( messageQueue->Tail <= messageQueue->Head )
    {
        pthread_mutex_unlock( &messageQueue->Mutex );
        return false;
    }
    *message = messageQueue->Messages[messageQueue->Head & ( MAX_MESSAGES - 1 )];
    messageQueue->Head++;
    pthread_mutex_unlock( &messageQueue->Mutex );
    if ( message->Wait == MQ_WAIT_RECEIVED )                                                        // if the message has a wait till received statues, set the received flag on the queue to be true
    {                                                                                               // and broadcast the received condition to whoever is listening
        messageQueue->ReceivedFlag = true;
        pthread_cond_broadcast( &messageQueue->ReceivedCondition );
    }
    else if ( message->Wait == MQ_WAIT_PROCESSED )                                                  // if the message has a wait till processed status, you set the message queue to a wait processed status
    {
        messageQueue->Wait = MQ_WAIT_PROCESSED;
    }
    return true;

}


/*
================================================================================

ovrAppThread

================================================================================
*/

enum
{
    MESSAGE_ON_CREATE,
    MESSAGE_ON_START,
    MESSAGE_ON_RESUME,
    MESSAGE_ON_PAUSE,
    MESSAGE_ON_STOP,
    MESSAGE_ON_DESTROY,
    MESSAGE_ON_SURFACE_CREATED,
    MESSAGE_ON_SURFACE_DESTROYED,
    MESSAGE_ON_KEY_EVENT,
    MESSAGE_ON_TOUCH_EVENT
};

typedef struct
{
    Image360*       Application;
    AAssetManager*  mgr;
    ovrMobile**      ovrM;
    bool            Resumed;
    pthread_t       Thread;
    ovrMessageQueue MessageQueue;
//    ANativeWindow*  NativeWindow;
} ovrAppThread;

// the ovr app running on a separate thread does its magic here
void * AppThreadFunction( void * parm ) {
    ovrAppThread *appThread = (ovrAppThread *) parm;

    auto renderer = (RendererGearVR*) appThread->Application->getRenderer();
    appThread->ovrM = renderer->getOvr();

    for (bool destroyed = false; destroyed == false; )                                              // initialize and cond test
    {
        for (; ;)                                                                                  // preferred over while(1)
        {
            ovrMessage message;
            const bool waitForMessages = (*appThread->ovrM == NULL && destroyed == false );
            if (!ovrMessageQueue_GetNextMessage(&appThread->MessageQueue, &message,
                                                waitForMessages)) {
                break;
            }

            switch (message.Id) {
                case MESSAGE_ON_CREATE: {

                    break;
                }
                case MESSAGE_ON_START: {
                    break;
                }
                case MESSAGE_ON_RESUME: {
                    appThread->Resumed = true;
                    break;
                }
                case MESSAGE_ON_PAUSE: {
                    appThread->Resumed = false;
                    appThread->Application->stop();
                    break;
                }
                case MESSAGE_ON_STOP: {
                    break;
                }
                case MESSAGE_ON_DESTROY: {
                    appThread->ovrM = NULL;             // how to handle this
                    destroyed = true;
//                    appThread->Application->deinitialize();
                    break;
                }
                case MESSAGE_ON_SURFACE_CREATED: {

                    appThread->Application->start();

                    ImageBMPLoaderAndroid imageBMPLoader(logger.get());
                    std::vector<std::unique_ptr<Image> > textureImages;
                    TEXTURE_MAP_MODE mode = CUBE_MAP_MODE_SIX_IMAGES; //EQUIRECTANGULAR_MAP_MODE;


                    switch (mode) {
                        case CUBE_MAP_MODE_SINGLE_IMAGE:
                            textureImages.push_back(imageBMPLoader.loadImage(appThread->mgr, "images/cubemap_current.bmp"));
                            break;
                        case CUBE_MAP_MODE_SIX_IMAGES:
                            textureImages.push_back(imageBMPLoader.loadImage(appThread->mgr, "images/cubemap_geo_front.bmp"));
                            textureImages.push_back(imageBMPLoader.loadImage(appThread->mgr, "images/cubemap_geo_left.bmp"));
                            textureImages.push_back(imageBMPLoader.loadImage(appThread->mgr, "images/cubemap_geo_back.bmp"));
                            textureImages.push_back(imageBMPLoader.loadImage(appThread->mgr, "images/cubemap_geo_right.bmp"));
                            textureImages.push_back(imageBMPLoader.loadImage(appThread->mgr, "images/cubemap_geo_top.bmp"));
                            textureImages.push_back(imageBMPLoader.loadImage(appThread->mgr, "images/cubemap_geo_bottom.bmp"));
                            break;
                        case EQUIRECTANGULAR_MAP_MODE:
                            textureImages.push_back(imageBMPLoader.loadImage(appThread->mgr, "images/tex_current.bmp"));
                            break;
                    }

                    appThread->Application->initialize(mode, textureImages);

//                    appThread->Resumed = true;
                    appThread->Application->draw();
                    break;
                }
                case MESSAGE_ON_SURFACE_DESTROYED: {
//                    appThread->Application->stop();
                    appThread->Application->deinitialize();
//
                    ANativeWindow_release(renderer->getWindow());
                    renderer->setWindow(NULL);
                    break;
                }
                case MESSAGE_ON_KEY_EVENT: {
                    break;
                }
                case MESSAGE_ON_TOUCH_EVENT: {
                    break;
                }

            }
        }

        if(*appThread->ovrM == NULL)
        {
            continue;
        }

        if(appThread->Resumed)
        {
            appThread->Application->draw();
        }

    }
    return NULL;
}

static void ovrAppThread_Create( ovrAppThread * appThread, Image360* application, AAssetManager* assetManager)
{

    appThread->Thread = 0;                                  // is this initialization necessary?
    appThread->Application = application;
    appThread->mgr = assetManager;
//    appThread->NativeWindow = NULL;
    appThread->Resumed = false;
    appThread->ovrM = NULL;
    ovrMessageQueue_Create( &appThread->MessageQueue );     // lets just assume the message queue works for now

    const int createErr = pthread_create( &appThread->Thread, NULL, AppThreadFunction, appThread );
    if (createErr != 0)
    {
        ALOGE("pthread_create returned %i", createErr);
    }
}

static void ovrAppThread_Destroy( ovrAppThread * appThread, JNIEnv * env )

{
    pthread_join( appThread->Thread, NULL );
    ovrMessageQueue_Destroy( &appThread->MessageQueue );
}


/*
====================================================================================================

                                         ACTIVITY LIFECYLE

====================================================================================================
*/

std::unique_ptr<Image360> application = nullptr;
//AAssetManager *mgr = nullptr;
pthread_t renderingThread;
static volatile bool isEnabled = false;

JNIEXPORT jlong JNICALL
Java_com_chymeravr_appgearvr_ActivityGearVR_onCreateNative(JNIEnv *env, jobject obj,
                                                           jobject activity, jobject assetManager) {
    ovrAppThread *appThread = (ovrAppThread *) malloc(sizeof(ovrAppThread));
    // allocate memory for an ovrAppThread

    loggerFactory = std::unique_ptr<LoggerFactoryAndroid>(new LoggerFactoryAndroid());
    logger = loggerFactory->createLogger("Image360::Native Android");
    logger->log(LOG_DEBUG, "Native Logger Created Successfully");

    auto mgr = AAssetManager_fromJava(env, assetManager);

    /* GL Model Factories for the application */
    std::unique_ptr<SceneGLFactory> sceneFactory(new SceneGLFactory(loggerFactory.get()));
    std::unique_ptr<ModelGLFactory> modelFactory(new ModelGLFactory(loggerFactory.get()));

    /* need a separate Camera for OVR due to the way it gives us access to view and projection
     * matrices for each of the eye
     */
    std::unique_ptr<CameraGLOVRFactory> cameraFactory(new CameraGLOVRFactory(loggerFactory.get()));

    /*
     * Separate Texture Factories for OpenGLES - shader language is separate
     */
    std::unique_ptr<DiffuseTextureGLES3Factory> diffuseTextureFactory(new DiffuseTextureGLES3Factory(loggerFactory.get()));
    std::unique_ptr<DiffuseTextureCubeMapGLES3Factory> diffuseTextureCubeMapFactory(new DiffuseTextureCubeMapGLES3Factory(loggerFactory.get()));

    std::unique_ptr<RendererGearVR> renderer(new RendererGearVR(env, activity, loggerFactory.get()));

    std::unique_ptr<TransformCameraFactory> transformCameraFactory(new TransformCameraFactory(loggerFactory.get()));
    std::unique_ptr<TransformModelFactory> transformModelFactory(new TransformModelFactory(loggerFactory.get()));


    std::unique_ptr<MutexLockAndroid> mutexLock(new MutexLockAndroid);
    eventQueue = std::unique_ptr<IEventQueue>(new EventQueue(std::move(mutexLock)));

    auto Application = new Image360(std::move(renderer),
                                    std::move(sceneFactory),
                                    std::move(modelFactory),
                                    std::move(diffuseTextureFactory),
                                    std::move(diffuseTextureCubeMapFactory),
                                    std::move(transformCameraFactory),
                                    std::move(transformModelFactory),
                                    std::move(cameraFactory),
                                    eventQueue.get(),
                                    loggerFactory.get());

    ovrAppThread_Create(appThread, Application, mgr);

    ovrMessageQueue_Enable( &appThread->MessageQueue, true);
    ovrMessage message;
    ovrMessage_Init(&message, MESSAGE_ON_CREATE, MQ_WAIT_PROCESSED);
    ovrMessageQueue_PostMessage( &appThread->MessageQueue, &message );

    return (jlong) ((size_t) appThread);

}

JNIEXPORT void JNICALL
Java_com_chymeravr_appgearvr_ActivityGearVR_onStartNative(JNIEnv *env, jobject obj, jobject activity, jlong handle) {

    ovrAppThread *appThread = (ovrAppThread*) ((size_t) handle);
    logger->log(LOG_DEBUG, "onStartNative()");


    ovrMessage message;
    ovrMessage_Init( &message, MESSAGE_ON_START, MQ_WAIT_PROCESSED);
    ovrMessageQueue_PostMessage( &appThread->MessageQueue, &message);
}

JNIEXPORT void JNICALL
Java_com_chymeravr_appgearvr_ActivityGearVR_onResumeNative(JNIEnv *env, jobject obj, jlong handle) {
    ovrAppThread *appThread = (ovrAppThread *) ((size_t) handle);
    logger->log(LOG_DEBUG, "onResumeNative()");

    isEnabled = true;
    auto renderer = (RendererGearVR*)appThread->Application->getRenderer();

    if( renderer->getWindow() != NULL ) {
//        const int createErr = pthread_create(&renderingThread, NULL, renderFunc,
//                                             (void *) application.get());
//        if (createErr != 0) {
//            logger->log(LOG_DEBUG, "pthread_create returned errir");
//        }
    }

    ovrMessage message;
    ovrMessage_Init( &message, MESSAGE_ON_RESUME, MQ_WAIT_PROCESSED );
    ovrMessageQueue_PostMessage( &appThread->MessageQueue, &message );
}

JNIEXPORT void JNICALL
Java_com_chymeravr_appgearvr_ActivityGearVR_onPauseNative(JNIEnv *env, jobject obj, jlong handle) {
    ovrAppThread *appThread = (ovrAppThread *) ((size_t) handle);
    logger->log(LOG_DEBUG, "onPauseNative()");

    isEnabled = false;

    ovrMessage message;
    ovrMessage_Init( &message, MESSAGE_ON_PAUSE, MQ_WAIT_PROCESSED );
    ovrMessageQueue_PostMessage( &appThread->MessageQueue, &message );
}

JNIEXPORT void JNICALL
Java_com_chymeravr_appgearvr_ActivityGearVR_onStopNative(JNIEnv *env, jobject obj, jlong handle) {
    ovrAppThread *appThread = (ovrAppThread *) ((size_t) handle);
    logger->log(LOG_DEBUG, "onStopNative()");

//    application->stop();

    ovrMessage message;
    ovrMessage_Init( &message, MESSAGE_ON_STOP, MQ_WAIT_PROCESSED );
    ovrMessageQueue_PostMessage( &appThread->MessageQueue, &message );

}

JNIEXPORT void JNICALL
Java_com_chymeravr_appgearvr_ActivityGearVR_onDestroyNative(JNIEnv *env, jobject obj,
                                                            jlong handle) {
    ovrAppThread *appThread = (ovrAppThread *) ((size_t) handle);
    logger->log(LOG_DEBUG, "onDestroyNative()");

    ovrMessage message;
    ovrMessage_Init( &message, MESSAGE_ON_DESTROY, MQ_WAIT_PROCESSED );
    ovrMessageQueue_PostMessage( &appThread->MessageQueue, &message );
    ovrMessageQueue_Enable( &appThread->MessageQueue, false );

    ovrAppThread_Destroy( appThread, env );
    free( appThread );
}

/*
====================================================================================================

                                         SURFACE LIFECYLE

====================================================================================================
*/

JNIEXPORT void JNICALL
Java_com_chymeravr_appgearvr_ActivityGearVR_onSurfaceCreatedNative(JNIEnv *env, jobject obj,
                                                                   jlong handle, jobject surface) {
    ovrAppThread *appThread = (ovrAppThread *) ((size_t) handle);
    logger->log(LOG_DEBUG, "onSurfaceCreatedNative() Begin");


    ANativeWindow *newNativeWindow = ANativeWindow_fromSurface(env, surface);

    if (ANativeWindow_getWidth(newNativeWindow) < ANativeWindow_getHeight(newNativeWindow)) {
        // An app that is relaunched after pressing the home button gets an initial surface
        // the orientation even though android:screenOrientation = "landscape" is set in the
        // manifest. The choreographer callback will also never be called for this surface because
        // The surface is immediately replaced with a new surface with the correct orientation.
        logger->log(LOG_ERROR, " Surface not in landscape model! ");
    }
    logger->log(LOG_DEBUG,
                "       NativeWindow = ANativeWIndow_fromSurface( env, surface )");
    auto renderer = (RendererGearVR *) appThread->Application->getRenderer();
    renderer->setWindow(newNativeWindow);

//    app->start();
//
//    /*
//     * Rendering to Surface must happen on a separate thread - not only is it bad practice not to
//     * do so, Oculus VRAPI will not function
//     */

//    appThread->NativeWindow = newNativeWindow;
    ovrMessage message;
    ovrMessage_Init( &message, MESSAGE_ON_SURFACE_CREATED, MQ_WAIT_PROCESSED);
    ovrMessage_SetPointerParm( &message, 0, newNativeWindow);
    ovrMessageQueue_PostMessage( &appThread->MessageQueue, &message);
}

JNIEXPORT void JNICALL
Java_com_chymeravr_appgearvr_ActivityGearVR_onSurfaceChangedNative(JNIEnv *env, jobject obj,
                                                                   jlong handle, jobject surface) {
    ovrAppThread *appThread = (ovrAppThread *) ((size_t) handle);
    logger->log(LOG_DEBUG, "onSurfaceChangedNative()");

    ANativeWindow* newNativeWindow = ANativeWindow_fromSurface( env, surface );
    if ( ANativeWindow_getWidth( newNativeWindow ) < ANativeWindow_getHeight( newNativeWindow) )
    {
        // An app that is relaunched after pressing the home button gets an initial surface with
        // the wrong orientation even though android:screenOrientation="landscape" is set in the
        // manifest. The choreographer callback will also never be called for this surface because
        // the surface is immediately replaced with a new surface with the correct orientation.
        ALOGE( "        Surface not in landscape mode!" );
    }

    auto renderer = (RendererGearVR *) appThread->Application->getRenderer();
    renderer->setWindow(newNativeWindow);
    auto currentWindow = renderer->getWindow();
    if ( newNativeWindow != currentWindow)                                                // we have a new window
    {
        if ( currentWindow != NULL)                                                       // there is already a window in this appthread - release it from your slavery !!!
        {
            ovrMessage message;
            ovrMessage_Init( &message, MESSAGE_ON_SURFACE_DESTROYED, MQ_WAIT_PROCESSED );
            ovrMessageQueue_PostMessage( &appThread->MessageQueue, &message );
            ALOGV( "        ANativeWindow_release( NativeWindow )" );
        }
        if ( newNativeWindow != NULL)                                                               // the new window is not null - wohoO!!
        {
            ALOGV( "         NativeWindow = ANativeWindow_fromSurface( env, surface ) " );
            ovrMessage message;
            ovrMessage_Init( &message, MESSAGE_ON_SURFACE_CREATED, MQ_WAIT_PROCESSED );
            ovrMessage_SetPointerParm( &message, 0, newNativeWindow );
            ovrMessageQueue_PostMessage( &appThread->MessageQueue, &message );
        }
    }
    else if (newNativeWindow != NULL )
    {
        ANativeWindow_release( newNativeWindow );
    }
}

JNIEXPORT void JNICALL
Java_com_chymeravr_appgearvr_ActivityGearVR_onSurfaceDestroyedNative(JNIEnv *env, jobject obj,
                                                                     jlong handle,
                                                                     jobject surface) {
    ovrAppThread *appThread = (ovrAppThread *) ((size_t) handle);
    logger->log(LOG_DEBUG, "onSurfaceDestroyedNative()");

    logger->log(LOG_INFO, "        ANativeWindow_release( NativeWindow )");

    ovrMessage message;
    ovrMessage_Init( &message, MESSAGE_ON_SURFACE_DESTROYED, MQ_WAIT_PROCESSED );
    ovrMessageQueue_PostMessage( &appThread->MessageQueue, &message );
//    ANativeWindow_release( appThread->NativeWindow );
//    appThread->NativeWindow = NULL;
}

#ifdef __cplusplus
}
#endif


/*
====================================================================================================

                                        INPUT

====================================================================================================
 */

JNIEXPORT void JNICALL
Java_com_chymeravr_appgearvr_ActivityGearVR_onKeyEventNative(JNIEnv *env, jobject obj, jlong handle,
                                                             int keyCode, int action) {
    if ( action == AKEY_EVENT_ACTION_UP )
    {
        ALOGV( "    GLES3JNILib::onKeyEvent( %d, %d )", keyCode, action );
    }
    ovrAppThread * appThread = (ovrAppThread *)((size_t)handle);
    ovrMessage message;
    ovrMessage_Init( &message, MESSAGE_ON_KEY_EVENT, MQ_WAIT_NONE );
    ovrMessage_SetIntegerParm( &message, 0, keyCode );
    ovrMessage_SetIntegerParm( &message, 1, action );
    ovrMessageQueue_PostMessage( &appThread->MessageQueue, &message );
}

JNIEXPORT void JNICALL
Java_com_chymeravr_appgearvr_ActivityGearVR_onTouchEventNative(JNIEnv *env, jobject obj,
                                                               jlong handle, int action, float x,
                                                               float y) {
    if ( action == AMOTION_EVENT_ACTION_UP )
    {
        ALOGV( "    GLES3JNILib::onTouchEvent( %d, %1.0f, %1.0f )", action, x, y );
    }
    ovrAppThread * appThread = (ovrAppThread *)((size_t)handle);
    ovrMessage message;
    ovrMessage_Init( &message, MESSAGE_ON_TOUCH_EVENT, MQ_WAIT_NONE );
    ovrMessage_SetIntegerParm( &message, 0, action );
    ovrMessage_SetFloatParm( &message, 1, x );
    ovrMessage_SetFloatParm( &message, 2, y );
    ovrMessageQueue_PostMessage( &appThread->MessageQueue, &message );
}