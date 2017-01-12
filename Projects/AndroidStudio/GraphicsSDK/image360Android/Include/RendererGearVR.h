//
// Created by robin_chimera on 1/9/2017.
//

#ifndef GRAPHICSSDK_RENDERERGEARVR_H
#define GRAPHICSSDK_RENDERERGEARVR_H

#include <cstdlib>
#include <string>
#include <cmath>
#include <ctime>
#include <sys/prctl.h>
#include <unistd.h>
//#include <commonFramework/include/Renderer.h>
#include <coreEngine/IRenderer.h>
#include <android/native_window.h> // requires ndk r5 or newer
#include <android/native_window_jni.h> // requires ndk r5 or newer
//#include <commonFramework/include/Logger.h>
#include <coreEngine/util/ILogger.h>
//#include <androidImplementations/include/LoggerAndroidImpl.h>

#include <android/log.h>

#include <VrApi.h>
#include <VrApi_Helpers.h>
//#include <SystemActivities.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

namespace cl {
#if !defined( EGL_OPENGL_ES3_BIT_KHR )
#define EGL_OPENGL_ES3_BIT_KHR		0x0040
#endif

#if !defined( GL_EXT_multisampled_render_to_texture )

    typedef void (GL_APIENTRY *PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC)(GLenum target,
                                                                           GLsizei samples,
                                                                           GLenum internalformat,
                                                                           GLsizei width,
                                                                           GLsizei height);

    typedef void (GL_APIENTRY *PFNGLFRAMEBUFFERTEXTURE2DMULTISAMPLEEXTPROC)(GLenum target,
                                                                            GLenum attachment,
                                                                            GLenum textarget,
                                                                            GLuint texture,
                                                                            GLint level,
                                                                            GLsizei samples);

#endif

#if !defined( GL_OVR_multiview )
    static const int GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_NUM_VIEWS_OVR = 0x9630;
    static const int GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_BASE_VIEW_INDEX_OVR = 0x9632;
    static const int GL_MAX_VIEWS_OVR = 0x9631;

    typedef void (GL_APIENTRY *PFNGLFRAMEBUFFERTEXTUREMULTIVIEWOVRPROC)(GLenum target,
                                                                        GLenum attachment,
                                                                        GLuint texture, GLint level,
                                                                        GLint baseViewIndex,
                                                                        GLsizei numViews);

#endif

#if !defined( GL_OVR_multiview_multisampled_render_to_texture )

    typedef void (GL_APIENTRY *PFNGLFRAMEBUFFERTEXTUREMULTISAMPLEMULTIVIEWOVRPROC)(GLenum target,
                                                                                   GLenum attachment,
                                                                                   GLuint texture,
                                                                                   GLint level,
                                                                                   GLsizei samples,
                                                                                   GLint baseViewIndex,
                                                                                   GLsizei numViews);

#endif

// Must use EGLSyncKHR because the VrApi still supports OpenGL ES 2.0
#define EGL_SYNC

#define DEBUG 1
#define LOG_TAG "Image360Native"

#define ALOGE(...) __android_log_print( ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__ )
#if DEBUG
#define ALOGV(...) __android_log_print( ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__ )
#else
#define ALOGV(...)
#endif

    // framebuffers
    static const char * GlFrameBufferStatusString( GLenum status )
    {
        switch ( status )
        {
            case GL_FRAMEBUFFER_UNDEFINED:						return "GL_FRAMEBUFFER_UNDEFINED";
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:			return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:	return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
            case GL_FRAMEBUFFER_UNSUPPORTED:					return "GL_FRAMEBUFFER_UNSUPPORTED";
            case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:			return "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
            default:											return "unknown";
        }
    }

#define CHECK_GL_ERRORS

#ifdef CHECK_GL_ERRORS

    static const char * GlErrorString( GLenum error )
{
    switch ( error )
    {
        case GL_NO_ERROR:						return "GL_NO_ERROR";
        case GL_INVALID_ENUM:					return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE:					return "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION:				return "GL_INVALID_OPERATION";
        case GL_INVALID_FRAMEBUFFER_OPERATION:	return "GL_INVALID_FRAMEBUFFER_OPERATION";
        case GL_OUT_OF_MEMORY:					return "GL_OUT_OF_MEMORY";
        default:                                return "unknown";
    }
}

static void GLCheckErrors( int line )
{
    for ( int i = 0; i < 10; i++ )
    {
        const GLenum error = glGetError();
        if ( error == GL_NO_ERROR )
        {
            break;
        }
        ALOGE( "GL error on line %d: %s", line, GlErrorString( error ) );
    }
}

#define GL( func )		func; GLCheckErrors( __LINE__ );

#else // CHECK_GL_ERRORS

#define GL(func)        func;

#endif // CHECK_GL_ERRORS

    static const int CPU_LEVEL = 2;
    static const int GPU_LEVEL = 3;
    static const int NUM_MULTI_SAMPLES = 4;

#define MULTI_THREADED			0
#define REDUCED_LATENCY			0

    typedef struct
    {
#if defined( EGL_SYNC )
        EGLDisplay	Display;
        EGLSyncKHR	Sync;
#else
        GLsync		Sync;
#endif
    } ovrFence;

    typedef struct
    {
        int						Width;
        int						Height;
        int						Multisamples;
        int						TextureSwapChainLength;
        int						TextureSwapChainIndex;
        bool					UseMultiview;
        ovrTextureSwapChain *	ColorTextureSwapChain;
        GLuint *				DepthBuffers;
        GLuint *				FrameBuffers;
    } ovrFramebuffer;

    typedef struct
    {
        ovrVector3f			CurrentRotation;
    } ovrSimulation;

    typedef struct
    {
        ovrFramebuffer	FrameBuffer[VRAPI_FRAME_LAYER_EYE_MAX];
        ovrFence *		Fence[VRAPI_FRAME_LAYER_EYE_MAX];
        ovrMatrix4f		ProjectionMatrix;
        ovrMatrix4f		TexCoordsTanAnglesMatrix;
        int				NumBuffers;
    } ovrRenderer;

    class RendererGearVR : public IRenderer {
        class OpenGLExtensions_t;
        class EGLParams;

        class OpenGLExtensions_t {
        public:
            bool multiView;
        public:
            void eglInitExtensions();
        };

        class EGLParams {
        public:
            EGLDisplay display;
            EGLConfig config;
            EGLint numConfigs;
            EGLSurface tinySurface;
            EGLSurface mainSurface;
            EGLContext context;
            EGLint width;
            EGLint height;
            EGLint majorVersion;
            EGLint minorVersion;
        private:
            ILogger *logger;
        public:
            EGLParams();

            void clear();

            void createEGLContext();

            void destroyEGLContext();

            static std::string getEglErrorString(const EGLint error);
        };


    private:
        std::unique_ptr<ILogger> logger;
        ANativeWindow *window;
        ovrJava java;
        JavaVM *javaVM;
        jobject activityObject;
        ovrMobile *ovrM;
        ovrPerformanceParms perfParms;

        OpenGLExtensions_t glExtensions;
        EGLParams* eglParams;
        ovrRenderer OVRRenderer;
        ovrSimulation OVRSimulation;

        bool useMultiView;
        long long frameIndex;
        int minimumVSyncs;

        ILoggerFactory *loggerFactory;

    public:
        RendererGearVR(JNIEnv *env, jobject activityObject, ILoggerFactory* loggerFactory);
        bool start();
        bool initialize(Scene* scene);
        void update();
        void draw(Scene* scene);
        void deinitialize(Scene* scene);
        void stop();
        void setWindow(ANativeWindow *window);
        ANativeWindow* getWindow();
    private:
        void enterIntoVrMode();
        void leaveVrMode();
    };
}

#endif //GRAPHICSSDK_RENDERERGEARVR_H
