////
//// Created by chimeralabs on 9/15/2016.
////
//
//#ifndef ANDROIDSDK_RENDERERGEARVR_H
//#define ANDROIDSDK_RENDERERGEARVR_H
//
//#include <cstdlib>
//#include <string>
//#include <cmath>
//#include <ctime>
//#include <sys/prctl.h>
//#include <unistd.h>
//#include <commonFramework/include/Renderer.h>
//#include <android/native_window.h> // requires ndk r5 or newer
//#include <android/native_window_jni.h> // requires ndk r5 or newer
//#include <commonFramework/include/Logger.h>
//#include <androidImplementations/include/LoggerAndroidImpl.h>
//
//#include <VrApi.h>
//#include <VrApi_Helpers.h>
//#include <SystemActivities.h>
//
//#include <EGL/egl.h>
//#include <EGL/eglext.h>
//#include <GLES3/gl3.h>
//#include <GLES3/gl3ext.h>
//
//namespace CL {
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
//
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
//        default: return "unknown";
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
//#define REDUCED_LATENCY 0
//
//    class RendererGearVR : public Renderer {
//        class OpenGLExtensions_t;
//        class EGLParams;
//        class OvrFrameBuffer;
//        class OvrRenderer;
//        class OvrFence;
//        class OvrSimulation;
//
//        class OpenGLExtensions_t {
//        public:
//            bool multiView;
//        public:
//            void eglInitExtensions();
//        };
//
//        class EGLParams {
//        public:
//            EGLDisplay display;
//            EGLConfig config;
//            EGLint numConfigs;
//            EGLSurface surface;
//            EGLSurface tinySurface;
//            EGLContext context;
//            EGLint width;
//            EGLint height;
//            EGLint majorVersion;
//            EGLint minorVersion;
//        private:
//            Logger *logger;
//        public:
//            EGLParams();
//
//            void clear();
//
//            void createEGLContext();
//
//            void destroyEGLContext();
//
//            static std::string getEglErrorString(const EGLint error);
//        };
//
//        class OvrFrameBuffer {
//        public:
//            int width;
//            int height;
//            int multisamples;
//            int textureSwapChainLength;
//            int textureSwapChainIndex;
//            bool useMultiView;
//            ovrTextureSwapChain *colorTextureSwapChain;
//            GLuint *depthBuffers;
//            GLuint *frameBuffers;
//        private:
//            Logger *logger;
//        public:
//            OvrFrameBuffer();
//            void clear();
//            bool create(const bool useMultiview, const ovrTextureFormat colorFormat,
//                        const int width, const int height, const int multisamples);
//            void destroy();
//            void setCurrent();
//            static void setNone();
//            void resolve();
//            void advance();
//            static std::string frameBufferStatusString(GLenum error);
//        };
//
//        class OvrRenderer {
//        public:
//            OvrFrameBuffer frameBuffer[VRAPI_FRAME_LAYER_EYE_MAX];
//            OvrFence *fence[VRAPI_FRAME_LAYER_EYE_MAX];
//            ovrMatrix4f projectionMatrix;
//            ovrMatrix4f texCoordsTanAnglesMatrix;
//            int numBuffers;
//        private:
//            Logger *logger;
//        public:
//            OvrRenderer();
//
//            void clear();
//
//            void create(const ovrJava *java, const bool useMultiview);
//
//            void destroy();
//
//            ovrFrameParms renderFrame(const ovrJava *java, long long frameIndex,
//                                      int minimumVsyncs,
//                                      const ovrPerformanceParms *perfParms,
//                                      const OvrSimulation *simulation,
//                                      const ovrTracking *tracking, ovrMobile *ovr);
//        };
//
//        class OvrFence {
//        public:
//#if defined( EGL_SYNC )
//            EGLDisplay	display;
//            EGLSyncKHR	sync;
//#else
//            GLsync sync;
//#endif
//        public:
//            void create();
//            void destroy();
//            void insert();
//        };
//
//        class OvrSimulation {
//            ovrVector3f currentRotation;
//        public:
//            void clear();
//            void advance(double predictedDisplayTime);
//        };
//
//    private:
//        Logger *logger;
//        ANativeWindow *window;
//        ovrJava java;
//        JavaVM *javaVM;
//        jobject activityObject;
//        ovrMobile *ovrM;
//        ovrPerformanceParms perfParms;
//
//        OpenGLExtensions_t glExtensions;
//        EGLParams eglParams;
//        OvrRenderer renderer;
//        OvrSimulation ovrSimulation;
//
//        bool useMultiView;
//        long long frameIndex;
//        int minimumVSyncs;
//    public:
//        RendererGearVR(JNIEnv *env, jobject activityObject);
//        bool start();
//        bool initialize();
//        void update();
//        void draw();
//        void destroy();
//        void setWindow(ANativeWindow *window);
//    private:
//        void enterIntoVrMode();
//        void leaveVrMode();
//    };
//}
//
//#endif //ANDROIDSDK_RENDERERGEARVR_H
