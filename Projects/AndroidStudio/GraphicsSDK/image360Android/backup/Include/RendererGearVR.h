//
// Created by robin_chimera on 1/2/2017.
//

#ifndef GRAPHICSSDK_RENDERERGEARVR_H
#define GRAPHICSSDK_RENDERERGEARVR_H

#include <cstdlib>
#include <string>
#include <cmath>
#include <ctime>
#include <sys/prctl.h>
#include <unistd.h>
#include <coreEngine/IRenderer.h>
#include <android/native_window.h> // requires ndk r5 or newer
#include <android/native_window_jni.h> // requires ndk r5 or newer
#include <coreEngine/util/ILogger.h>
#include <LoggerAndroid.h>

#include "OpenGLExtensions.h"
#include "OvrFence.h"
#include "OvrSimulation.h"
#include "OvrRenderer.h"
#include "EGLParams.h"

#include <VrApi.h>
#include <VrApi_Helpers.h>
//#include <SystemActivities.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

namespace cl {

    class RendererGearVR : public IRenderer{
    private:
        std::unique_ptr<ILogger> logger;
        ANativeWindow *window;
        ovrJava java;
        JavaVM *javaVM;
        jobject activityObject;
        ovrMobile *ovrM;
        ovrPerformanceParms perfParms;

        OpenGLExtensions *glExtensions;
        EGLParams *eglParams;
        OvrRenderer *renderer;
        OvrSimulation *ovrSimulation;

        bool useMultiView;
        long long frameIndex;
        int minimumVSyncs;

        ILoggerFactory* loggerFactory;


    public:
        RendererGearVR(JNIEnv *env, jobject activityObject, ILoggerFactory* loggerFactory);
        bool start();
        bool initialize(Scene *scene);
        void update();
        void draw(Scene *scene);
        void deinitialize(Scene *scene);
        void stop();
        void setWindow(ANativeWindow *window);
        ANativeWindow* getWindow();

//        void drawScene(Scene *scene);
    private:
        void enterIntoVrMode();
        void leaveVrMode();

    };
}


#endif //GRAPHICSSDK_RENDERERGEARVR_H
