//
// Created by robin_chimera on 1/7/2017.
//

#include <LoggerAndroid.h>
#include <VrApi_Types.h>
#include <VrApi_Helpers.h>
#include <VrApi.h>
#include <OvrFrameBuffer.h>
#include <OvrFence.h>
#include "OvrRenderer.h"
#include <OvrSimulation.h>
#include <coreEngine/renderObjects/Scene.h>

#include "OvrFrameBuffer.h"

#include <GLES3/gl3.h>
#include <coreEngine/renderObjects/Camera.h>
#include <coreEngine/components/transform/Transform.h>
#include <CameraGLOVR.h>
#include <RendererGearVR.h>
#include <coreEngine/renderObjects/Shader.h>
#include <coreEngine/renderObjects/Material.h>
#include <coreEngine/renderObjects/Model.h>

namespace cl {
    /**
*
* OvrRenderer class methods
*
*/
    static const int NUM_MULTI_SAMPLES = 4;

    OvrRenderer::OvrRenderer()
            : logger(new LoggerAndroid("RendererGearVR::OvrRenderer"))
    {
    }

    void OvrRenderer::clear() {

        for (int eye = 0; eye < VRAPI_FRAME_LAYER_EYE_MAX; eye++) {
            //this->clear();
            this->frameBuffer[eye].clear();         // this is how you would clear each framebuffer
            //ovrFramebuffer_Clear( &renderer->FrameBuffer[eye] );
        }
        this->projectionMatrix = ovrMatrix4f_CreateIdentity();
        this->texCoordsTanAnglesMatrix = ovrMatrix4f_CreateIdentity();
        this->numBuffers = VRAPI_FRAME_LAYER_EYE_MAX;
    }

    void OvrRenderer::create(const ovrJava *java, const bool useMultiview) {
        this->numBuffers = useMultiview ? 1 : VRAPI_FRAME_LAYER_EYE_MAX;

        //create frame buffers
        for (int eye = 0; eye < this->numBuffers; eye++) {
            this->frameBuffer[eye].create(useMultiview,
                                          VRAPI_TEXTURE_FORMAT_8888,
                                          vrapi_GetSystemPropertyInt(java,
                                                                     VRAPI_SYS_PROP_SUGGESTED_EYE_TEXTURE_WIDTH),
                                          vrapi_GetSystemPropertyInt(java,
                                                                     VRAPI_SYS_PROP_SUGGESTED_EYE_TEXTURE_HEIGHT),
                                          NUM_MULTI_SAMPLES);
            this->fence[eye] = (OvrFence *) malloc(
                    this->frameBuffer[eye].textureSwapChainLength * sizeof(OvrFence));
            for (int i = 0; i < this->frameBuffer[eye].textureSwapChainLength; i++) {
                this->fence[eye][i].create();
            }
        }

        //setup projection matrix
        this->projectionMatrix = ovrMatrix4f_CreateProjectionFov(
                vrapi_GetSystemPropertyFloat(java, VRAPI_SYS_PROP_SUGGESTED_EYE_FOV_DEGREES_X),
                vrapi_GetSystemPropertyFloat(java, VRAPI_SYS_PROP_SUGGESTED_EYE_FOV_DEGREES_Y),
                0.0f, 0.0f, 1.0f, 0.0f);
        this->texCoordsTanAnglesMatrix = ovrMatrix4f_TanAngleMatrixFromProjection(
                &this->projectionMatrix);
    }

    void OvrRenderer::destroy() {
        for (int eye = 0; eye < this->numBuffers; eye++) {
            for (int i = 0; i < this->frameBuffer[eye].textureSwapChainLength; i++) {
                this->fence[eye][i].destroy();
            }
            free(this->fence[eye]);

            this->frameBuffer[eye].destroy();
        }
        this->projectionMatrix = ovrMatrix4f_CreateIdentity();
        this->texCoordsTanAnglesMatrix = ovrMatrix4f_CreateIdentity();
    }
    ovrFrameParms OvrRenderer::renderFrame(const ovrJava *java, long long frameIndex,
                                           int minimumVsyncs, const ovrPerformanceParms *perfParms,
                                           const OvrSimulation *simulation, Scene* scene,
                                           const ovrTracking *tracking, ovrMobile *ovr)
    {
        ovrFrameParms parms = vrapi_DefaultFrameParms(java, VRAPI_FRAME_INIT_DEFAULT, vrapi_GetTimeInSeconds(), NULL);
        parms.FrameIndex = frameIndex;
        parms.MinimumVsyncs = minimumVsyncs;
        parms.PerformanceParms = *perfParms;

//        const ovrHeadModelParms headModelParms = vrapi_DefaultHeadModelParms();

#if REDUCED_LATENCY
        // Update orientation, not position.
	ovrTracking updatedTracking = vrapi_GetPredictedTracking( ovr, tracking->HeadPose.TimeInSeconds );
	updatedTracking.HeadPose.Pose.Position = tracking->HeadPose.Pose.Position;
#else
        ovrTracking updatedTracking = *tracking;
#endif

        // Calculate the view matrix.
//        const ovrMatrix4f centerEyeViewMatrix = vrapi_GetCenterEyeViewMatrix( &headModelParms, &updatedTracking, NULL );
//
//        ovrMatrix4f eyeViewMatrix[2];
//        eyeViewMatrix[0] = vrapi_GetEyeViewMatrix( &headModelParms, &centerEyeViewMatrix, 0 );
//        eyeViewMatrix[1] = vrapi_GetEyeViewMatrix( &headModelParms, &centerEyeViewMatrix, 1 );
//
//        ovrMatrix4f eyeViewMatrixTransposed[2];
//        eyeViewMatrixTransposed[0] = ovrMatrix4f_Transpose( &eyeViewMatrix[0] );
//        eyeViewMatrixTransposed[1] = ovrMatrix4f_Transpose( &eyeViewMatrix[1] );
//
//        ovrMatrix4f projectionMatrixTransposed;
//        projectionMatrixTransposed = ovrMatrix4f_Transpose( &this->projectionMatrix );

//        // Update the scene matrices.
//        std::vector<Relation*> cameraRelations = scene->getRelations("camera");
////        Transform* transform = (Transform*)(((Camera*)cameraRelations[0])->getComponentList().getComponent("transform"));
//        CameraGLOVR *camera = (CameraGLOVR*)cameraRelations[0];

        // we need to write out the transformation matrices into the scene
//        ovrMatrix4f sceneMatrices = {1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0};



        for ( int eye = 0; eye < VRAPI_FRAME_LAYER_EYE_MAX; eye++)
        {
            OvrFrameBuffer* frameBuffer = &this->frameBuffer[this->numBuffers == 1 ? 0 : eye];      //&renderer->FrameBuffer[renderer->NumBuffers == 1 ? 0 : eye];
            parms.Layers[0].Textures[eye].ColorTextureSwapChain = frameBuffer->colorTextureSwapChain;
            parms.Layers[0].Textures[eye].TextureSwapChainIndex = frameBuffer->textureSwapChainIndex;
            parms.Layers[0].Textures[eye].TexCoordsFromTanAngles = this->texCoordsTanAnglesMatrix;
        }

        parms.Layers[0].Flags |= VRAPI_FRAME_LAYER_FLAG_CHROMATIC_ABERRATION_CORRECTION;

        unsigned long long completionFence[VRAPI_FRAME_LAYER_EYE_MAX] = { 0 };

        // Render the eye images.
        for ( int eye = 0; eye < this->numBuffers; eye++)
        {
            OvrFrameBuffer* frameBuffer = &(this->frameBuffer[eye]);
            frameBuffer->setCurrent();

//            auto tempViewMat = eyeViewMatrix[eye];
//            CL_Mat44 sceneViewMat = { tempViewMat.M[0][0], tempViewMat.M[0][1], tempViewMat.M[0][2], tempViewMat.M[0][3],
//                                      tempViewMat.M[1][0], tempViewMat.M[1][1], tempViewMat.M[1][2], tempViewMat.M[1][3],
//                                      tempViewMat.M[2][0], tempViewMat.M[2][1], tempViewMat.M[2][2], tempViewMat.M[2][3],
//                                      tempViewMat.M[3][0], tempViewMat.M[3][1], tempViewMat.M[3][2], tempViewMat.M[3][3]};
//            camera->setViewMatrix(sceneViewMat);
//
//            auto tempProjectionMat = eyeViewMatrix[eye];
//            CL_Mat44 sceneProjectionMat = { tempProjectionMat.M[0][0], tempProjectionMat.M[0][1], tempProjectionMat.M[0][2], tempProjectionMat.M[0][3],
//                                            tempProjectionMat.M[1][0], tempProjectionMat.M[1][1], tempProjectionMat.M[1][2], tempProjectionMat.M[1][3],
//                                            tempProjectionMat.M[2][0], tempProjectionMat.M[2][1], tempProjectionMat.M[2][2], tempProjectionMat.M[2][3],
//                                            tempProjectionMat.M[3][0], tempProjectionMat.M[3][1], tempProjectionMat.M[3][2], tempProjectionMat.M[3][3]};
//            camera->setProjectionMatrix(sceneProjectionMat);

            GL( glEnable( GL_SCISSOR_TEST ) );
            GL( glDepthMask( GL_TRUE ) );
            GL( glEnable( GL_DEPTH_TEST ) );
            GL( glDepthFunc( GL_LEQUAL ) );
            GL( glEnable( GL_CULL_FACE ) );
            GL( glCullFace( GL_BACK ) );
            GL( glViewport( 0, 0, frameBuffer->width, frameBuffer->height ) );
            GL( glScissor( 0, 0, frameBuffer->width, frameBuffer->height ) );
            GL( glClearColor( 0.75f, 0.32f, 0.45f, 1.0f ) );
            GL( glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) );

//            GL (this->drawScene(scene) );

            // Explicitly clear the border texels to black because OpenGL-ES does not support GL_CLAMP_TO_BORDER.
            {
                // Clear to fully opaque black.
                GL( glClearColor( 0.60f, 0.40f, 0.30f, 1.0f ) );
                // bottom
                GL( glScissor( 0, 0, frameBuffer->width, 1 ) );
                GL( glClear( GL_COLOR_BUFFER_BIT ) );
                // top
                GL( glScissor( 0, frameBuffer->height - 1, frameBuffer->width, 1 ) );
                GL( glClear( GL_COLOR_BUFFER_BIT ) );
                // left
                GL( glScissor( 0, 0, 1, frameBuffer->height ) );
                GL( glClear( GL_COLOR_BUFFER_BIT ) );
                // right
                GL( glScissor( frameBuffer->width - 1, 0, 1, frameBuffer->height ) );
                GL( glClear( GL_COLOR_BUFFER_BIT ) );
            }

            frameBuffer->resolve();

            OvrFence* fence = &this->fence[eye][frameBuffer->textureSwapChainIndex];//&renderer->Fence[eye][frameBuffer->TextureSwapChainIndex];
            //ovrFence_Insert( fence );
            fence->insert();
            //completionFence[eye] = (size_t)fence->Sync;
            completionFence[eye] = (size_t)fence->sync;

            //ovrFramebuffer_Advance( frameBuffer );
            frameBuffer->advance();
        }

        for ( int eye = 0; eye < VRAPI_FRAME_LAYER_EYE_MAX; eye++ )
        {
            parms.Layers[0].Textures[eye].CompletionFence =
                    completionFence[this->numBuffers == 1 ? 0 : eye];
        }

        //ovrFramebuffer_SetNone();
        frameBuffer->setNone();

        return parms;
    }

    void OvrRenderer::drawScene(Scene* scene)
    {
        IRenderable *sceneRenderer = scene->getRenderable();
        sceneRenderer->draw();

        std::vector<Relation*> cameraRelations = scene->getRelations("camera");
        assert(cameraRelations.size() == 1);
        ((Camera*)cameraRelations[0])->getRenderable()->draw();

        std::vector<Relation*> shaderRelations = scene->getRelations("shader");
        for (auto it = shaderRelations.cbegin(); it != shaderRelations.cend(); it++){
            Shader* shader = (Shader*)(*it);
            shader->getRenderable()->draw();

            std::vector<Relation*> materialRelations = shader->getRelations("material");
            for (auto it = materialRelations.cbegin(); it != materialRelations.cend(); it++){
                Material *material = (Material*)(*it);
                material->getRenderable()->draw();

                std::vector<Relation*> modelRelations = material->getRelations("model");
                for (auto it = modelRelations.cbegin(); it != modelRelations.cend(); it++){
                    Model *model = (Model*)(*it);
                    model->getRenderable()->draw();
                }
            }
        }
    }



//    ovrFrameParms OvrRenderer::renderFrame(const ovrJava *java, long long frameIndex,
//                                           int minimumVsyncs, const ovrPerformanceParms *perfParms,
//                                           const OvrSimulation *simulation, Scene* scene,
//                                           const ovrTracking *tracking, ovrMobile *ovr) {
//        ovrFrameParms parms = vrapi_DefaultFrameParms(java, VRAPI_FRAME_INIT_DEFAULT, vrapi_GetTimeInSeconds(), NULL);
//        parms.FrameIndex = frameIndex;
//        parms.MinimumVsyncs = minimumVsyncs;
//        parms.PerformanceParms = *perfParms;
//        parms.Layers[VRAPI_FRAME_LAYER_TYPE_WORLD].Flags |= VRAPI_FRAME_LAYER_FLAG_CHROMATIC_ABERRATION_CORRECTION;
//
//        const ovrHeadModelParms headModelParms = vrapi_DefaultHeadModelParms();
//
//#if REDUCED_LATENCY
//        ovrTracking updatedTracking = vrapi_GetPredictedTracking(ovr, tracking->HeadPose.TimeInSeconds);
//        updatedTracking.HeadPose.Pose.Position = tracking->HeadPose.Pose.Position;
//#else
//        ovrTracking updatedTracking = *tracking;
//#endif
//
//        // Calculate the view matrix.
//        const ovrMatrix4f centerEyeViewMatrix = vrapi_GetCenterEyeViewMatrix( &headModelParms, &updatedTracking, NULL );
//
//        ovrMatrix4f eyeViewMatrix[2];
//        eyeViewMatrix[0] = vrapi_GetEyeViewMatrix( &headModelParms, &centerEyeViewMatrix, 0 );
//        eyeViewMatrix[1] = vrapi_GetEyeViewMatrix( &headModelParms, &centerEyeViewMatrix, 1 );
//
//        ovrMatrix4f eyeViewMatrixTransposed[2];
//        eyeViewMatrixTransposed[0] = ovrMatrix4f_Transpose( &eyeViewMatrix[0] );
//        eyeViewMatrixTransposed[1] = ovrMatrix4f_Transpose( &eyeViewMatrix[1] );
//
//        ovrMatrix4f projectionMatrixTransposed;
//        projectionMatrixTransposed = ovrMatrix4f_Transpose(&this->projectionMatrix);
//
//        for(int eye=0; eye < VRAPI_FRAME_LAYER_EYE_MAX; eye++){
//            OvrFrameBuffer *frameBuffer = &this->frameBuffer[this->numBuffers == 1 ? 0 : eye];
//            parms.Layers[VRAPI_FRAME_LAYER_TYPE_WORLD].Textures[eye].ColorTextureSwapChain = frameBuffer->colorTextureSwapChain;
//            parms.Layers[VRAPI_FRAME_LAYER_TYPE_WORLD].Textures[eye].TextureSwapChainIndex = frameBuffer->textureSwapChainIndex;
//            parms.Layers[VRAPI_FRAME_LAYER_TYPE_WORLD].Textures[eye].TexCoordsFromTanAngles = this->texCoordsTanAnglesMatrix;
//            parms.Layers[VRAPI_FRAME_LAYER_TYPE_WORLD].Textures[eye].HeadPose = updatedTracking.HeadPose;
//        }
//
//        unsigned long long completionFence[VRAPI_FRAME_LAYER_EYE_MAX] = { 0 };
//
//        for(int eye=0; eye<this->numBuffers; eye++){
//            // NOTE: In the non-mv case, latency can be further reduced by updating the sensor prediction
//            // for each eye (updates orientation, not position)
//            OvrFrameBuffer * frameBuffer = &this->frameBuffer[eye];
//            frameBuffer->setCurrent();
//            GL( glEnable( GL_SCISSOR_TEST ) );
//            GL( glDepthMask( GL_TRUE ) );
//            GL( glEnable( GL_DEPTH_TEST ) );
//            GL( glDepthFunc( GL_LEQUAL ) );
//            GL( glEnable( GL_CULL_FACE ) );
//            GL( glCullFace( GL_BACK ) );
//            GL( glViewport( 0, 0, frameBuffer->width, frameBuffer->height ) );
//            GL( glScissor( 0, 0, frameBuffer->width, frameBuffer->height ) );
//            GL( glClearColor( 0.125f, 0.0f, 0.125f, 1.0f ) );
//            GL( glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) );
//            //GL( glBindVertexArray( scene->Cube.VertexArrayObject ) );
//            //GL( glDrawElementsInstanced( GL_TRIANGLES, scene->Cube.IndexCount, GL_UNSIGNED_SHORT, NULL, NUM_INSTANCES ) );
//            //GL( glBindVertexArray( 0 ) );
//
//
//            // Explicitly clear the border texels to black because OpenGL-ES does not support GL_CLAMP_TO_BORDER.
//            {
//                // Clear to fully opaque black.
//                GL( glClearColor( 0.0f, 0.0f, 0.0f, 1.0f ) );
//                // bottom
//                GL( glScissor( 0, 0, frameBuffer->width, 1 ) );
//                GL( glClear( GL_COLOR_BUFFER_BIT ) );
//                // top
//                GL( glScissor( 0, frameBuffer->height - 1, frameBuffer->width, 1 ) );
//                GL( glClear( GL_COLOR_BUFFER_BIT ) );
//                // left
//                GL( glScissor( 0, 0, 1, frameBuffer->height ) );
//                GL( glClear( GL_COLOR_BUFFER_BIT ) );
//                // right
//                GL( glScissor( frameBuffer->width - 1, 0, 1, frameBuffer->height ) );
//                GL( glClear( GL_COLOR_BUFFER_BIT ) );
//            }
//
//            frameBuffer->resolve();
//            OvrFence * fence = &this->fence[eye][frameBuffer->textureSwapChainIndex];
//            fence->insert();
//            completionFence[eye] = (size_t)fence->sync;
//
//            frameBuffer->advance();
//        }
//        for ( int eye = 0; eye < VRAPI_FRAME_LAYER_EYE_MAX; eye++ )
//        {
//            parms.Layers[VRAPI_FRAME_LAYER_TYPE_WORLD].Textures[eye].CompletionFence =
//                    completionFence[this->numBuffers == 1 ? 0 : eye];
//        }
//        OvrFrameBuffer::setNone();
//
//        return parms;
//    }


};