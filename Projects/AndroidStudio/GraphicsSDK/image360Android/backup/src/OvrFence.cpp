//
// Created by robin_chimera on 1/7/2017.
//

#include <GLES3/gl3.h>
#include <EGL/eglext.h>
#include <OvrFrameBuffer.h>
#include "OvrFence.h"

namespace cl {
/**
    *
    * OvrFence class methods
    *
    */
    void OvrFence::create() {
#if defined( EGL_SYNC )
        this->display = 0;
        this->sync = EGL_NO_SYNC_KHR;
#else
        this->sync = 0;
#endif
    }

    void OvrFence::destroy() {
#if defined( EGL_SYNC )
        if (this->sync != EGL_NO_SYNC_KHR) {
                if (eglDestroySyncKHR(this->display, this->sync) == EGL_FALSE) {
                    //ALOGE( "eglDestroySyncKHR() : EGL_FALSE" );
                    return;
                }
                this->display = 0;
                this->sync = EGL_NO_SYNC_KHR;
            }
#else
        if (this->sync != 0) {
            glDeleteSync(this->sync);
            this->sync = 0;
        }
#endif
    }

    void OvrFence::insert() {
        this->destroy();
#if defined( EGL_SYNC )
        this->display = eglGetCurrentDisplay();
            this->sync = eglCreateSyncKHR(this->display, EGL_SYNC_FENCE_KHR, NULL);
            if (this->sync == EGL_NO_SYNC_KHR) {
                //ALOGE("eglCreateSyncKHR() : EGL_NO_SYNC_KHR");
                return;
            }
            // Force flushing the commands.
            // Note that some drivers will already flush when calling eglCreateSyncKHR.
            if (eglClientWaitSyncKHR(this->display, this->sync, EGL_SYNC_FLUSH_COMMANDS_BIT_KHR, 0) ==
                EGL_FALSE) {
                //ALOGE("eglClientWaitSyncKHR() : EGL_FALSE");
                return;
            }
#else
        // Create and insert a new sync object.
        this->sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
        // Force flushing the commands.
        // Note that some drivers will already flush when calling glFenceSync.
        glClientWaitSync(this->sync, GL_SYNC_FLUSH_COMMANDS_BIT, 0);
#endif

    }

}