//
// Created by robin_chimera on 1/7/2017.
//

#ifndef GRAPHICSSDK_OVRRENDERER_H
#define GRAPHICSSDK_OVRRENDERER_H

#include <LoggerAndroid.h>
#include <VrApi_Types.h>
#include <VrApi_Helpers.h>
#include <VrApi.h>
#include <OvrFrameBuffer.h>
#include <OvrFence.h>
#include "OvrRenderer.h"
#include <OvrSimulation.h>
#include <coreEngine/renderObjects/Scene.h>

namespace cl {

    // Must use EGLSyncKHR because the VrApi still supports OpenGL ES 2.0

    class OvrRenderer {
    public:
        OvrFrameBuffer frameBuffer[VRAPI_FRAME_LAYER_EYE_MAX];
        OvrFence *fence[VRAPI_FRAME_LAYER_EYE_MAX];
        ovrMatrix4f projectionMatrix;
        ovrMatrix4f texCoordsTanAnglesMatrix;
        int numBuffers;
    private:
        ILogger *logger;
    public:
        OvrRenderer();

        void clear();

        void create(const ovrJava *java, const bool useMultiview);

        void destroy();

        ovrFrameParms renderFrame(const ovrJava *java, long long frameIndex,
                                  int minimumVsyncs, const ovrPerformanceParms *perfParms,
                                  const OvrSimulation *simulation, Scene* scene,
                                  const ovrTracking *tracking,
                                  ovrMobile *ovr);

    private:
        void drawScene(Scene *scene);
    };
}
#endif //GRAPHICSSDK_OVRRENDERER_H
