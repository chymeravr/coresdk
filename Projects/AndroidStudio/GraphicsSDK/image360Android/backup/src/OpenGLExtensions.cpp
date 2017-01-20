//
// Created by robin_chimera on 1/7/2017.
//

#include <GLES3/gl3.h>
#include <memory.h>
#include "OpenGLExtensions.h"

namespace cl {
/**
     *
     * OpenglExtensions_t class
     *
     */
    void OpenGLExtensions::eglInitExtensions() {
#if defined EGL_SYNC
        eglCreateSyncKHR = (PFNEGLCREATESYNCKHRPROC) eglGetProcAddress("eglCreateSyncKHR");
        eglDestroySyncKHR = (PFNEGLDESTROYSYNCKHRPROC) eglGetProcAddress("eglDestroySyncKHR");
        eglClientWaitSyncKHR = (PFNEGLCLIENTWAITSYNCKHRPROC) eglGetProcAddress(
                "eglClientWaitSyncKHR");
        eglSignalSyncKHR = (PFNEGLSIGNALSYNCKHRPROC) eglGetProcAddress("eglSignalSyncKHR");
        eglGetSyncAttribKHR = (PFNEGLGETSYNCATTRIBKHRPROC) eglGetProcAddress("eglGetSyncAttribKHR");
#endif

        // get extension pointers
        const char *allExtensions = (const char *) glGetString(GL_EXTENSIONS);
        if (allExtensions != NULL) {
            this->multiView = strstr(allExtensions, "GL_OVR_multiview2") &&
                              strstr(allExtensions,
                                     "GL_OVR_multiview_multisampled_render_to_texture");
        }
    }

}