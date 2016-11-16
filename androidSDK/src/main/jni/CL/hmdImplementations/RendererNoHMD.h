//
// Created by chimeralabs on 9/7/2016.
//

#ifndef ANDROIDSDK_RENDERERNOHMD_H
#define ANDROIDSDK_RENDERERNOHMD_H

#include <memory>
#include <vector>
#include <android/native_window.h> // requires ndk r5 or newer
#include <android/native_window_jni.h> // requires ndk r5 or newer
#include <GLES2/gl2.h>

#include <EGL/egl.h>
#include <coreEngine/model/IRenderer.h>
#include <coreEngine/util/Logger.h>
#include <glImplementation/service/IServiceGLPool.h>

namespace cl{
    class RendererNoHMD : public IRenderer{
        class EGLParams{
        public:
            EGLDisplay display;
            EGLConfig config;
            EGLint numConfigs;
            EGLSurface surface;
            EGLContext context;
            EGLint width;
            EGLint height;
            EGLint majorVersion;
            EGLint minorVersion;
        };
    private:
        EGLParams eglParams;
        ANativeWindow *window;
        Logger *loggerPtr;
        IServiceGLPool *serviceGLPoolPtr;
        std::unique_ptr<ISceneService> sceneServicePtr;
        std::unique_ptr<IShaderService> shaderServicePtr;
        std::unique_ptr<IMaterialService> materialServicePtr;
        std::unique_ptr<IModelService> modelServicePtr;

    public:
        RendererNoHMD();
        ~RendererNoHMD();

        //Renderer virtual method implementations
        bool start();
        bool initialize(Scene &scene);
        void update();
        void draw(Scene &scene);
        void deinitialize(Scene &scene);
        void destroy();
        void setWindow(ANativeWindow *window);
    };
}

#endif //ANDROIDSDK_RENDERERNOHMD_H
