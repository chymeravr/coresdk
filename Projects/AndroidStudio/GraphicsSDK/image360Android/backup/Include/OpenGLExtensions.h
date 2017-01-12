//
// Created by robin_chimera on 1/7/2017.
//

#ifndef GRAPHICSSDK_OPENGLEXTENSIONS_H
#define GRAPHICSSDK_OPENGLEXTENSIONS_H


namespace cl {
    class OpenGLExtensions {
    public:
        bool multiView;
    public:
        void eglInitExtensions();
    };
}
#endif //GRAPHICSSDK_OPENGLEXTENSIONS_H
