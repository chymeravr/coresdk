//
// Created by robin_chimera on 1/7/2017.
//

#ifndef GRAPHICSSDK_OVRFENCE_H
#define GRAPHICSSDK_OVRFENCE_H

namespace cl{

    class OvrFence {
    public:
#if defined( EGL_SYNC )
        EGLDisplay	display;
        EGLSyncKHR	sync;
#else
        GLsync sync;
#endif
    public:
        void create();
        void destroy();
        void insert();
    };
}
#endif //GRAPHICSSDK_OVRFENCE_H
