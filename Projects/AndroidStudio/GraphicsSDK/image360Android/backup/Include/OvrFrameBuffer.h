//
// Created by robin_chimera on 1/7/2017.
//

#ifndef GRAPHICSSDK_OVRFRAMEBUFFER_H
#define GRAPHICSSDK_OVRFRAMEBUFFER_H

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <VrApi_Types.h>

#include <coreEngine/util/ILogger.h>

namespace cl {

#if !defined( GL_EXT_multisampled_render_to_texture )

    typedef void (GL_APIENTRY* PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC)(GLenum target,
                                                                           GLsizei samples,
                                                                           GLenum internalformat,
                                                                           GLsizei width,
                                                                           GLsizei height);

    typedef void (GL_APIENTRY* PFNGLFRAMEBUFFERTEXTURE2DMULTISAMPLEEXTPROC)(GLenum target,
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

    typedef void (GL_APIENTRY* PFNGLFRAMEBUFFERTEXTUREMULTIVIEWOVRPROC)(GLenum target,
                                                                        GLenum attachment,
                                                                        GLuint texture, GLint level,
                                                                        GLint baseViewIndex,
                                                                        GLsizei numViews);

#endif

#if !defined( GL_OVR_multiview_multisampled_render_to_texture )

    typedef void (GL_APIENTRY* PFNGLFRAMEBUFFERTEXTUREMULTISAMPLEMULTIVIEWOVRPROC)(GLenum target,
                                                                                   GLenum attachment,
                                                                                   GLuint texture,
                                                                                   GLint level,
                                                                                   GLsizei samples,
                                                                                   GLint baseViewIndex,
                                                                                   GLsizei numViews);

#endif

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
        default: return "unknown";
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



#define REDUCED_LATENCY 0


    class OvrFrameBuffer
    {
    public:
        int width;
        int height;
        int multisamples;
        int textureSwapChainLength;
        int textureSwapChainIndex;
        bool useMultiView;
        ovrTextureSwapChain *colorTextureSwapChain;
        GLuint *depthBuffers;
        GLuint *frameBuffers;
    private:
        ILogger *logger;
    public:
        OvrFrameBuffer();
        void clear();
        bool create(const bool useMultiview, const ovrTextureFormat colorFormat,
                    const int width, const int height, const int multisamples);
        void destroy();
        void setCurrent();
        static void setNone();
        void resolve();
        void advance();
        static std::string frameBufferStatusString(GLenum error);
    };
}
#endif //GRAPHICSSDK_OVRFRAMEBUFFER_H
