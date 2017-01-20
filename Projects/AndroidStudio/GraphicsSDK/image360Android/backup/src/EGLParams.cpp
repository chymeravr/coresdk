//
// Created by robin_chimera on 1/7/2017.
//

/**
 *
 * EGLParams class methods
 *
 */
#include <string>
#include <EGL/eglplatform.h>
#include <EGL/egl.h>
#include "EGLParams.h"

#include <coreEngine/util/ILogger.h>
#include <GLES3/gl3.h>
#include <LoggerAndroid.h>

namespace cl {

    EGLParams::EGLParams()
            : logger(new LoggerAndroid("RendererGearVR::OvrFrameBuffer"))
    {
    }


    void EGLParams::clear() {
        this->majorVersion = 0;
        this->minorVersion = 0;
        this->display = 0;
        this->config = 0;
        this->numConfigs = 0;
        this->tinySurface = EGL_NO_SURFACE;
        this->surface = EGL_NO_SURFACE;
        this->context = EGL_NO_CONTEXT;
        this->width = 0;
        this->height = 0;
    }

    void EGLParams::createEGLContext() {

        if( this->display != 0)             // must create context only once
        {
            return;
        }

        this->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        this->logger->log(LOG_DEBUG, "EGL initialized");

        eglInitialize(this->display, &this->majorVersion, &this->minorVersion);
        // Do NOT use eglChooseConfig, because the Android EGL code pushes in multisample
        // flags in eglChooseConfig if the user has selected the "force 4x MSAA" option in
        // settings, and that is completely wasted for our warp target.

        const int MAX_CONFIGS = 1024;
        EGLConfig configs[MAX_CONFIGS];
        EGLint numConfigs = 0;

        if (eglGetConfigs(this->display, configs, MAX_CONFIGS, &numConfigs) == EGL_FALSE) {
            this->logger->log(LOG_ERROR,
                        "eglGetConfigs() failed: " + this->getEglErrorString(glGetError()));
        }

        const EGLint configAttribs[] =
                {
                        EGL_RED_SIZE, 8,
                        EGL_GREEN_SIZE, 8,
                        EGL_BLUE_SIZE, 8,
                        EGL_ALPHA_SIZE, 8, // need alpha for the multi-pass timewarp compositor
                        EGL_DEPTH_SIZE, 0,
                        EGL_STENCIL_SIZE, 0,
                        EGL_SAMPLES, 0,
                        EGL_NONE
                };
        this->config = 0;

        for (int i = 0; i < numConfigs; i++) {
            EGLint value = 0;

            eglGetConfigAttrib(this->display, configs[i], EGL_RENDERABLE_TYPE, &value);
            if ((value & EGL_OPENGL_ES3_BIT_KHR) != EGL_OPENGL_ES3_BIT_KHR) {
                continue;
            }

            // The pbuffer config also needs to be compatible with normal window rendering
            // so it can share textures with the window context.
            eglGetConfigAttrib(this->display, configs[i], EGL_SURFACE_TYPE, &value);
            if ((value & (EGL_WINDOW_BIT | EGL_PBUFFER_BIT)) !=
                (EGL_WINDOW_BIT | EGL_PBUFFER_BIT)) {
                continue;
            }

            int j = 0;
            for (; configAttribs[j] != EGL_NONE; j += 2) {
                eglGetConfigAttrib(display, configs[i], configAttribs[j], &value);
                if (value != configAttribs[j + 1]) {
                    break;
                }
            }
            if (configAttribs[j] == EGL_NONE) {
                this->config = configs[i];
                break;
            }
        }
        if (this->config == 0) {
            this->logger->log(LOG_ERROR,
                        "eglChooseConfig() failed: " + this->getEglErrorString(eglGetError()));
            return;
        }
        EGLint contextAttribs[] =
                {
                        EGL_CONTEXT_CLIENT_VERSION, 3,
                        EGL_NONE
                };

        this->logger->log(LOG_DEBUG, "eglCreateContext");
        this->context = eglCreateContext(this->display, this->config, EGL_NO_CONTEXT, contextAttribs);

        if (this->context == EGL_NO_CONTEXT) {
            this->logger->log(LOG_ERROR,
                        "eglCreateContext() failed: " + this->getEglErrorString(eglGetError()));
            return;
        }

        const EGLint surfaceAttribs[] =
                {
                        EGL_WIDTH, 16,
                        EGL_HEIGHT, 16,
                        EGL_NONE
                };

        this->logger->log(LOG_DEBUG, "eglCreatePbufferSurface");
        this->tinySurface = eglCreatePbufferSurface(this->display, this->config, surfaceAttribs);

        if (this->tinySurface == EGL_NO_SURFACE) {
            this->logger->log(LOG_ERROR, "eglCreatePbufferSurface() failed: " +
                                   this->getEglErrorString(eglGetError()));
            eglDestroyContext(this->display, this->context);
            this->context = EGL_NO_CONTEXT;
            return;
        }

        this->logger->log(LOG_DEBUG, "eglMakeCurrent( Display, TinySurface, TinySurface, Context )");
        if (eglMakeCurrent(this->display, this->tinySurface, this->tinySurface, this->context) == EGL_FALSE) {
            this->logger->log(LOG_ERROR,
                        "eglMakeCurrent() failed: " + this->getEglErrorString(eglGetError()));
            eglDestroySurface(this->display, this->tinySurface);
            eglDestroyContext(this->display, this->context);
            this->context = EGL_NO_CONTEXT;
            return;
        }
    }

    void EGLParams::destroyEGLContext() {
        if (this->display != 0) {
            logger->log(LOG_DEBUG,
                        "eglMakeCurrent (display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT)");
            if (eglMakeCurrent(this->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT) ==
                EGL_FALSE) {
                logger->log(LOG_ERROR,
                            "eglMakeCurrent failed: " + this->getEglErrorString(eglGetError()));
            }
        }
        if (this->context != EGL_NO_CONTEXT) {
            logger->log(LOG_DEBUG, "eglDestroyContext(display, context)");
            if (eglDestroyContext(this->display, this->context) == EGL_FALSE) {
                logger->log(LOG_ERROR, "eglDestroyContext(display, context) failed: " +
                                       this->getEglErrorString(eglGetError()));
            }
            this->context = EGL_NO_CONTEXT;
        }
        if (this->tinySurface != EGL_NO_SURFACE) {
            logger->log(LOG_DEBUG, "eglDestroySurface(display, surface)");
            if (eglDestroySurface(this->display, this->surface) == EGL_FALSE) {
                logger->log(LOG_ERROR, "eglDestroySurface(display, surface) failed: " +
                                       this->getEglErrorString(eglGetError()));
            }
            this->tinySurface = EGL_NO_SURFACE;
        }
        if (this->display != 0) {
            logger->log(LOG_DEBUG, "eglTerminate(display)");
            if (eglTerminate(this->display) == EGL_FALSE) {
                logger->log(LOG_ERROR,
                            "eglTerminate(display) failed:" + this->getEglErrorString(eglGetError()));
            }
            this->display = 0;
        }
    }

    std::string EGLParams::getEglErrorString(const EGLint error) {
        switch (error) {
            case EGL_SUCCESS:
                return "EGL_SUCCESS";
            case EGL_NOT_INITIALIZED:
                return "EGL_NOT_INITIALIZED";
            case EGL_BAD_ACCESS:
                return "EGL_BAD_ACCESS";
            case EGL_BAD_ALLOC:
                return "EGL_BAD_ALLOC";
            case EGL_BAD_ATTRIBUTE:
                return "EGL_BAD_ATTRIBUTE";
            case EGL_BAD_CONTEXT:
                return "EGL_BAD_CONTEXT";
            case EGL_BAD_CONFIG:
                return "EGL_BAD_CONFIG";
            case EGL_BAD_CURRENT_SURFACE:
                return "EGL_BAD_CURRENT_SURFACE";
            case EGL_BAD_DISPLAY:
                return "EGL_BAD_DISPLAY";
            case EGL_BAD_SURFACE:
                return "EGL_BAD_SURFACE";
            case EGL_BAD_MATCH:
                return "EGL_BAD_MATCH";
            case EGL_BAD_PARAMETER:
                return "EGL_BAD_PARAMETER";
            case EGL_BAD_NATIVE_PIXMAP:
                return "EGL_BAD_NATIVE_PIXMAP";
            case EGL_BAD_NATIVE_WINDOW:
                return "EGL_BAD_NATIVE_WINDOW";
            case EGL_CONTEXT_LOST:
                return "EGL_CONTEXT_LOST";
            default:
                return "unknown";
        }
    }

}