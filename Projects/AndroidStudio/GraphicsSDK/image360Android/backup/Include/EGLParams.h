//
// Created by robin_chimera on 1/7/2017.
//

#ifndef GRAPHICSSDK_EGLPARAMS_H
#define GRAPHICSSDK_EGLPARAMS_H

#include <coreEngine/util/ILogger.h>
#include <LoggerAndroid.h>
#include <VrApi_Types.h>
#include <VrApi_Helpers.h>
#include <VrApi.h>
#include <OvrFrameBuffer.h>
#include <OvrFence.h>
#include "OvrRenderer.h"

namespace cl {
#if !defined( EGL_OPENGL_ES3_BIT_KHR )
#define EGL_OPENGL_ES3_BIT_KHR		0x0040
#endif

    class EGLParams {
    public:
        EGLint majorVersion;
        EGLint minorVersion;
        EGLDisplay display;
        EGLConfig config;
        EGLint numConfigs;
        EGLSurface tinySurface;
        EGLSurface surface;
        EGLContext context;
        EGLint width;
        EGLint height;

    private:
        ILogger *logger;
    public:
        EGLParams();

        void clear();

        void createEGLContext();

        void destroyEGLContext();

        static std::string getEglErrorString(const EGLint error);
    };

}

#endif //GRAPHICSSDK_EGLPARAMS_H
