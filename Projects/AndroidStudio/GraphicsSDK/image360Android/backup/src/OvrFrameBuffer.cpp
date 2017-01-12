//
// Created by robin_chimera on 1/7/2017.
//

#include <string>
#include <VrApi_Types.h>
#include <LoggerAndroid.h>
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <VrApi.h>
#include "OvrFrameBuffer.h"

namespace cl{
    /**
     *
     * OvrFrameRendere class methods
     *
     */
    OvrFrameBuffer::OvrFrameBuffer()
            : logger(new LoggerAndroid("RendererGearVR::OvrFrameBuffer"))
    {
    }

    void OvrFrameBuffer::clear() {
        this->width = 0;
        this->height = 0;
        this->multisamples = 0;
        this->textureSwapChainLength = 0;
        this->textureSwapChainIndex = 0;
        this->useMultiView = false;
        this->colorTextureSwapChain = NULL;
        this->depthBuffers = NULL;
        this->frameBuffers = NULL;
    }

    bool OvrFrameBuffer::create(const bool useMultiview,
                                                const ovrTextureFormat colorFormat, const int width,
                                                const int height, const int multisamples) {
        PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC glRenderbufferStorageMultisampleEXT =
                (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC) eglGetProcAddress(
                        "glRenderbufferStorageMultisampleEXT");
        PFNGLFRAMEBUFFERTEXTURE2DMULTISAMPLEEXTPROC glFramebufferTexture2DMultisampleEXT =
                (PFNGLFRAMEBUFFERTEXTURE2DMULTISAMPLEEXTPROC) eglGetProcAddress(
                        "glFramebufferTexture2DMultisampleEXT");

        PFNGLFRAMEBUFFERTEXTUREMULTIVIEWOVRPROC glFramebufferTextureMultiviewOVR =
                (PFNGLFRAMEBUFFERTEXTUREMULTIVIEWOVRPROC) eglGetProcAddress(
                        "glFramebufferTextureMultiviewOVR");
        PFNGLFRAMEBUFFERTEXTUREMULTISAMPLEMULTIVIEWOVRPROC glFramebufferTextureMultisampleMultiviewOVR =
                (PFNGLFRAMEBUFFERTEXTUREMULTISAMPLEMULTIVIEWOVRPROC) eglGetProcAddress(
                        "glFramebufferTextureMultisampleMultiviewOVR");

        this->width = width;
        this->height = height;
        this->multisamples = multisamples;
        this->useMultiView = useMultiview;

        this->colorTextureSwapChain = vrapi_CreateTextureSwapChain(
                this->useMultiView ? VRAPI_TEXTURE_TYPE_2D_ARRAY : VRAPI_TEXTURE_TYPE_2D,
                colorFormat, this->width, this->height, 1, true);
        this->textureSwapChainLength = vrapi_GetTextureSwapChainLength(this->colorTextureSwapChain);
        this->depthBuffers = (GLuint *) malloc(this->textureSwapChainLength * sizeof(GLuint));
        this->frameBuffers = (GLuint *) malloc(this->textureSwapChainLength * sizeof(GLuint));

        logger->log(LOG_DEBUG, "OvrFrameBuffer->useMultiView:" + this->useMultiView);

        for (int i = 0; i < this->textureSwapChainLength; i++) {
            //create color buffer texture
            const GLuint colorTexture = vrapi_GetTextureSwapChainHandle(this->colorTextureSwapChain,
                                                                        i);
            GLenum colorTextureTarget = this->useMultiView ? GL_TEXTURE_2D_ARRAY : GL_TEXTURE_2D;
            GL(glBindTexture(colorTextureTarget, colorTexture));
            GL(glTexParameteri(colorTextureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
            GL(glTexParameteri(colorTextureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
            GL(glTexParameteri(colorTextureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            GL(glTexParameteri(colorTextureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            GL(glBindTexture(colorTextureTarget, 0));

            if (this->useMultiView) {
                //Create depth buffer texture
                GL(glGenTextures(1, &this->depthBuffers[i]));
                GL(glBindTexture(GL_TEXTURE_2D_ARRAY, this->depthBuffers[i]));
                GL(glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_DEPTH_COMPONENT24, this->width,
                                  this->height, 2));
                GL(glBindTexture(GL_TEXTURE_2D_ARRAY, 0));

                //create Frame buffer
                GL(glGenFramebuffers(1, &this->frameBuffers[i]));
                GL(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->frameBuffers[i]));
                if (this->multisamples > 1 &&
                    (glFramebufferTextureMultisampleMultiviewOVR != NULL)) {
                    GL(glFramebufferTextureMultisampleMultiviewOVR(GL_DRAW_FRAMEBUFFER,
                                                                   GL_DEPTH_ATTACHMENT,
                                                                   this->depthBuffers[i],
                                                                   0 /* level */,
                                                                   this->multisamples /* samples */,
                                                                   0 /* baseViewIndex */,
                                                                   2 /* numViews */ ))
                    GL(glFramebufferTextureMultisampleMultiviewOVR(GL_DRAW_FRAMEBUFFER,
                                                                   GL_COLOR_ATTACHMENT0,
                                                                   colorTexture, 0 /* level */,
                                                                   multisamples /* samples */,
                                                                   0 /* baseViewIndex */,
                                                                   2 /* numViews */ ));
                } else {
                    GL(glFramebufferTextureMultiviewOVR(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                                        this->depthBuffers[i], 0 /* level */,
                                                        0 /* baseViewIndex */, 2 /* numViews */ ));
                    GL(glFramebufferTextureMultiviewOVR(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                                        colorTexture, 0 /* level */,
                                                        0 /* baseViewIndex */, 2 /* numViews */ ));
                }
                GL(GLenum renderFrameBufferStatus = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER));
                GL(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
                if (renderFrameBufferStatus != GL_FRAMEBUFFER_COMPLETE) {
                    logger->log(LOG_ERROR, "Incomplete frame buffer object: " +
                                           frameBufferStatusString(renderFrameBufferStatus));
                    return false;
                }

            } else { //useMultiView
                if (this->multisamples > 1 && glRenderbufferStorageMultisampleEXT != NULL &&
                    glFramebufferTexture2DMultisampleEXT != NULL) {
                    //create multisample depth buffer
                    GL(glGenRenderbuffers(1, &this->depthBuffers[i]));
                    GL(glBindRenderbuffer(GL_RENDERBUFFER, this->depthBuffers[i]));
                    GL(glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER, this->multisamples,
                                                           GL_DEPTH_COMPONENT24, width, height));
                    GL(glBindRenderbuffer(GL_RENDERBUFFER, 0));

                    //create framebuffer
                    GL(glGenFramebuffers(1, &this->frameBuffers[i]));
                    GL(glBindFramebuffer(GL_FRAMEBUFFER, this->frameBuffers[i]));
                    GL(glFramebufferTexture2DMultisampleEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                                            GL_TEXTURE_2D, colorTexture, 0,
                                                            this->multisamples));
                    GL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                                 GL_RENDERBUFFER, this->depthBuffers[i]));
                    GL(GLenum renderFrameBufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER));
                    GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
                    if (renderFrameBufferStatus != GL_FRAMEBUFFER_COMPLETE) {
                        logger->log(LOG_ERROR, "Incomplete frame buffer object:" +
                                               frameBufferStatusString(renderFrameBufferStatus));
                        return false;
                    }
                } else {
                    //create depth buffers
                    GL(glGenRenderbuffers(1, &this->depthBuffers[i]));
                    GL(glBindRenderbuffer(GL_RENDERBUFFER, this->depthBuffers[i]));
                    GL(glRenderbufferStorage(GL_FRAMEBUFFER, GL_DEPTH_COMPONENT24, width, height));
                    GL(glBindFramebuffer(GL_RENDERBUFFER, 0));

                    //create framebuffer
                    GL(glGenFramebuffers(1, &this->frameBuffers[i]));
                    GL(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->frameBuffers[i]));
                    GL(glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                                 GL_RENDERBUFFER, this->frameBuffers[i]));
                    GL(glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                              GL_TEXTURE_2D, colorTexture, 0));
                    GL(GLenum renderFrameBufferStatus = glCheckFramebufferStatus(
                            GL_DRAW_FRAMEBUFFER));
                    GL(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
                    if (renderFrameBufferStatus != GL_FRAMEBUFFER_COMPLETE) {
                        logger->log(LOG_ERROR, "Incomplete frame buffer object:" +
                                               frameBufferStatusString(renderFrameBufferStatus));
                        return false;
                    }
                }
            }

        }
        return true;
    }

    void OvrFrameBuffer::destroy() {
        GL(glDeleteFramebuffers(this->textureSwapChainLength, this->frameBuffers));
        if (this->useMultiView) {
            GL(glDeleteTextures(this->textureSwapChainLength, this->depthBuffers));
        }
        else {
            GL(glDeleteRenderbuffers(this->textureSwapChainLength, this->depthBuffers));
        }
        vrapi_DestroyTextureSwapChain(this->colorTextureSwapChain);

        free(this->depthBuffers);
        free(this->frameBuffers);

        this->clear();
    }

    void OvrFrameBuffer::setCurrent() {
        GL(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->frameBuffers[this->textureSwapChainIndex]));
    }

    void OvrFrameBuffer::setNone() {
        GL(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
    }

    void OvrFrameBuffer::resolve() {
        const GLenum depthAttachment[1] = {GL_DEPTH_ATTACHMENT};
        glInvalidateFramebuffer(GL_DRAW_FRAMEBUFFER, 1, depthAttachment);

        //Flush this frame worth of commands
        glFlush();
    }

    void OvrFrameBuffer::advance() {
        this->textureSwapChainIndex =
                (this->textureSwapChainIndex + 1) % this->textureSwapChainLength;
    }

    std::string OvrFrameBuffer::frameBufferStatusString(GLenum error) {
        switch ( error )
        {
            case GL_FRAMEBUFFER_UNDEFINED:						return "GL_FRAMEBUFFER_UNDEFINED";
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:			return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:	return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
            case GL_FRAMEBUFFER_UNSUPPORTED:					return "GL_FRAMEBUFFER_UNSUPPORTED";
            case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:			return "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
            default:											return "unknown";
        }
    }
}