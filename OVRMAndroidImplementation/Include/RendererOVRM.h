////
//// Created by robin_chimera on 1/9/2017.
////
//
//#ifndef ANDROIDIMPLEMENTATION_RENDEREROVRM_H
//#define ANDROIDIMPLEMENTATION_RENDEREROVRM_H
//
//#include <cstdlib>
//#include <string>
//#include <cmath>
//#include <ctime>
//#include <sys/prctl.h>
//#include <unistd.h>
//#include <coreEngine/IRenderer.h>
//#include <android/native_window.h>                                                                  // requires ndk r5 or newer
//#include <android/native_window_jni.h>                                                              // requires ndk r5 or newer
//#include <coreEngine/util/ILogger.h>
//
//#include <glImplementation/renderObjects/CameraGL.h>
//
//#include <android/log.h>
//
//#include <VrApi.h>
//#include <VrApi_Helpers.h>
//
//#include <EGL/egl.h>
//#include <EGL/eglext.h>
//#include <GLES3/gl3.h>
//#include <GLES3/gl3ext.h>
//
//#include <pthread.h>
//
//namespace cl {
//#if !defined( EGL_OPENGL_ES3_BIT_KHR )
//#define EGL_OPENGL_ES3_BIT_KHR		0x0040
//#endif
//
//#if !defined( GL_EXT_multisampled_render_to_texture )
//
//    typedef void (GL_APIENTRY *PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC)(GLenum target,
//                                                                           GLsizei samples,
//                                                                           GLenum internalformat,
//                                                                           GLsizei width,
//                                                                           GLsizei height);
//
//    typedef void (GL_APIENTRY *PFNGLFRAMEBUFFERTEXTURE2DMULTISAMPLEEXTPROC)(GLenum target,
//                                                                            GLenum attachment,
//                                                                            GLenum textarget,
//                                                                            GLuint texture,
//                                                                            GLint level,
//                                                                            GLsizei samples);
//
//#endif
//
//#if !defined( GL_OVR_multiview )
//    static const int GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_NUM_VIEWS_OVR = 0x9630;
//    static const int GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_BASE_VIEW_INDEX_OVR = 0x9632;
//    static const int GL_MAX_VIEWS_OVR = 0x9631;
//
//    typedef void (GL_APIENTRY *PFNGLFRAMEBUFFERTEXTUREMULTIVIEWOVRPROC)(GLenum target,
//                                                                        GLenum attachment,
//                                                                        GLuint texture, GLint level,
//                                                                        GLint baseViewIndex,
//                                                                        GLsizei numViews);
//
//#endif
//
//#if !defined( GL_OVR_multiview_multisampled_render_to_texture )
//
//    typedef void (GL_APIENTRY *PFNGLFRAMEBUFFERTEXTUREMULTISAMPLEMULTIVIEWOVRPROC)(GLenum target,
//                                                                                   GLenum attachment,
//                                                                                   GLuint texture,
//                                                                                   GLint level,
//                                                                                   GLsizei samples,
//                                                                                   GLint baseViewIndex,
//                                                                                   GLsizei numViews);
//
//#endif
//
//// Must use EGLSyncKHR because the VrApi still supports OpenGL ES 2.0
//#define EGL_SYNC
//
//#define DEBUG 1
//#define LOG_TAG "Image360OVRM"
//
//#define ALOGE(...) __android_log_print( ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__ )
//#if DEBUG
//#define ALOGV(...) __android_log_print( ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__ )
//#else
//#define ALOGV(...)
//#endif
//
//    // egl errors
//    static const char * EglErrorString( const EGLint error )
//    {
//        switch ( error )
//        {
//            case EGL_SUCCESS:				return "EGL_SUCCESS";
//            case EGL_NOT_INITIALIZED:		return "EGL_NOT_INITIALIZED";
//            case EGL_BAD_ACCESS:			return "EGL_BAD_ACCESS";
//            case EGL_BAD_ALLOC:				return "EGL_BAD_ALLOC";
//            case EGL_BAD_ATTRIBUTE:			return "EGL_BAD_ATTRIBUTE";
//            case EGL_BAD_CONTEXT:			return "EGL_BAD_CONTEXT";
//            case EGL_BAD_CONFIG:			return "EGL_BAD_CONFIG";
//            case EGL_BAD_CURRENT_SURFACE:	return "EGL_BAD_CURRENT_SURFACE";
//            case EGL_BAD_DISPLAY:			return "EGL_BAD_DISPLAY";
//            case EGL_BAD_SURFACE:			return "EGL_BAD_SURFACE";
//            case EGL_BAD_MATCH:				return "EGL_BAD_MATCH";
//            case EGL_BAD_PARAMETER:			return "EGL_BAD_PARAMETER";
//            case EGL_BAD_NATIVE_PIXMAP:		return "EGL_BAD_NATIVE_PIXMAP";
//            case EGL_BAD_NATIVE_WINDOW:		return "EGL_BAD_NATIVE_WINDOW";
//            case EGL_CONTEXT_LOST:			return "EGL_CONTEXT_LOST";
//            default:						return "unknown";
//        }
//    }
//
//    // framebuffers
//    static const char * GlFrameBufferStatusString( GLenum status )
//    {
//        switch ( status )
//        {
//            case GL_FRAMEBUFFER_UNDEFINED:						return "GL_FRAMEBUFFER_UNDEFINED";
//            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:			return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
//            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:	return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
//            case GL_FRAMEBUFFER_UNSUPPORTED:					return "GL_FRAMEBUFFER_UNSUPPORTED";
//            case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:			return "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
//            default:											return "unknown";
//        }
//    }
//
//#define CHECK_GL_ERRORS
//
//#ifdef CHECK_GL_ERRORS
//
//    static const char * GlErrorString( GLenum error )
//{
//    switch ( error )
//    {
//        case GL_NO_ERROR:						return "GL_NO_ERROR";
//        case GL_INVALID_ENUM:					return "GL_INVALID_ENUM";
//        case GL_INVALID_VALUE:					return "GL_INVALID_VALUE";
//        case GL_INVALID_OPERATION:				return "GL_INVALID_OPERATION";
//        case GL_INVALID_FRAMEBUFFER_OPERATION:	return "GL_INVALID_FRAMEBUFFER_OPERATION";
//        case GL_OUT_OF_MEMORY:					return "GL_OUT_OF_MEMORY";
//        default:                                return "unknown";
//    }
//}
//
//static void GLCheckErrors( int line )
//{
//    for ( int i = 0; i < 10; i++ )
//    {
//        const GLenum error = glGetError();
//        if ( error == GL_NO_ERROR )
//        {
//            break;
//        }
//        ALOGE( "GL error on line %d: %s", line, GlErrorString( error ) );
//    }
//}
//
//#define GL( func )		func; GLCheckErrors( __LINE__ );
//
//#else // CHECK_GL_ERRORS
//
//#define GL(func)        func;
//
//#endif // CHECK_GL_ERRORS
//
//    static const int CPU_LEVEL = 2;
//    static const int GPU_LEVEL = 3;
//    static const int NUM_MULTI_SAMPLES = 4;
//
//#define MULTI_THREADED			0
////#define REDUCED_LATENCY			0
//
//    typedef struct
//    {
//        bool multi_view;			// GL_OVR_multiview, GL_OVR_multiview2
//    } OpenGLExtensions_t;
//
//
//    typedef struct
//    {
//        EGLint		MajorVersion;
//        EGLint		MinorVersion;
//        EGLDisplay	Display;
//        EGLConfig	Config;
//        EGLSurface	TinySurface;
//        EGLSurface	MainSurface;
//        EGLContext	Context;
//    } ovrEgl;
//
//
//    typedef struct
//    {
//#if defined( EGL_SYNC )
//        EGLDisplay	Display;
//        EGLSyncKHR	Sync;
//#else
//        GLsync		Sync;
//#endif
//    } ovrFence;
//
//    typedef struct
//    {
//        int						Width;
//        int						Height;
//        int						Multisamples;
//        int						TextureSwapChainLength;
//        int						TextureSwapChainIndex;
//        bool					UseMultiview;
//        ovrTextureSwapChain *	ColorTextureSwapChain;
//        GLuint *				DepthBuffers;
//        GLuint *				FrameBuffers;
//    } ovrFramebuffer;
//
//    typedef struct
//    {
//        ovrVector3f			CurrentRotation;
//    } ovrSimulation;
//
//    typedef struct
//    {
//        ovrFramebuffer	FrameBuffer[VRAPI_FRAME_LAYER_EYE_MAX];
//        ovrFence *		Fence[VRAPI_FRAME_LAYER_EYE_MAX];
//        ovrMatrix4f		ProjectionMatrix;
//        ovrMatrix4f		TexCoordsTanAnglesMatrix;
//        int				NumBuffers;
//    } ovrRenderer;
//
//    class RendererOVRM : public IRenderer {
//
//    private:
//        std::unique_ptr<ILogger> logger;
//        ANativeWindow *window;
//        ovrJava java;
//        JavaVM *javaVM;
//        jobject activityObject;
//        ovrMobile *ovrM;
//        ovrPerformanceParms perfParms;
//
//        OpenGLExtensions_t glExtensions;
//        ovrEgl eglParams;
//        ovrRenderer OVRRenderer;
//        ovrSimulation OVRSimulation;
//
//        bool useMultiView;
//        long long frameIndex;
//        int minimumVSyncs;
//
//        //CameraGLOVR* renderCamera;
//        CameraGL* renderCamera;
//
//        ILoggerFactory *loggerFactory;
//
////    protected:
////        static void toEulerianAngle(const ovrQuatf& q, float& roll, float& pitch, float& yaw);
////        static CL_Vec3 getCameraLocation(const ovrHeadModelParms * headModelParms, const int eye);
//
//
//    public:
//        RendererOVRM(JNIEnv *env, jobject activityObject, ILoggerFactory* loggerFactory);
//        bool start();
//        bool initialize(Scene* scene);
//        void update();
//        void draw(Scene* scene);
//        void draw(Scene* scene, EYE eye);
//        void deinitialize(Scene* scene);
//        void stop();
//        void setWindow(ANativeWindow *window);
//        ANativeWindow* getWindow();
//        ovrMobile* getOvr();
////    private:
//        void enterIntoVrMode();
//        void leaveVrMode();
//        void handleVrModeChanges(bool resumed);
//        std::vector<float> getHMDParams();
//    };
//}
//
//#endif //ANDROIDIMPLEMENTATION_RENDEREROVRM_H
