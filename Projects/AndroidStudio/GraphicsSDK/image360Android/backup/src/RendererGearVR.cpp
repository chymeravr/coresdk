//
// Created by robin_chimera on 1/2/2017.
//

//#include <assert.h>
#include <VrApi_Types.h>
#include <VrApi.h>
#include <cassert>
#include <coreEngine/renderObjects/Camera.h>
#include <coreEngine/renderObjects/Shader.h>
#include <coreEngine/renderObjects/Material.h>
#include <coreEngine/renderObjects/Model.h>
#include <LoggerAndroid.h>
#include <EGLParams.h>
#include <OpenGLExtensions.h>
#include <OvrFrameBuffer.h>
#include <OvrRenderer.h>
#include <OvrSimulation.h>

#include "../Include/RendererGearVR.h"

namespace cl {

    static const int CPU_LEVEL = 2;
    static const int GPU_LEVEL = 3;


    /**
     *
     * RendererGearVR class methods
     *
     */

    RendererGearVR::RendererGearVR(JNIEnv *env, jobject activityObject, ILoggerFactory* loggerFactory)
            : window(NULL), ovrM(NULL), frameIndex(0), minimumVSyncs(1)
    {
        this->logger = std::unique_ptr<ILogger>(loggerFactory->createLogger("RendererGearVR"));

        this->eglParams = new EGLParams();
        this->eglParams->clear();

        this->glExtensions = new OpenGLExtensions();

        this->renderer = new OvrRenderer();
        this->renderer->clear();

        this->ovrSimulation = new OvrSimulation();
        this->ovrSimulation->clear();

        //logger->log(LOG_INFO, "Renderer created");
        //(*env)->GetJavaVM( env, &this->javaVM);
        env->GetJavaVM(&this->javaVM);
        this->java.Env = env;
        this->activityObject = env->NewGlobalRef(activityObject);
        //this->java.Vm = this->javaVM;

    }

    bool RendererGearVR::start() {
        this->java.Vm = this->javaVM;
        this->java.Vm->AttachCurrentThread(&this->java.Env, NULL);
        this->java.ActivityObject = this->activityObject;

        prctl(PR_SET_NAME, (long) "CL::Main", 0, 0, 0);
        logger->log(LOG_DEBUG, "SystemActivities_Init()");
        //SystemActivities_Init(&java);

        logger->log(LOG_DEBUG, "vrapi_DefaultInitParms()");
        const ovrInitParms initParms = vrapi_DefaultInitParms(&this->java);
        logger->log(LOG_DEBUG, "vrapi_Initialize()");
        int32_t initResult = vrapi_Initialize(&initParms);

        if (initResult != VRAPI_INITIALIZE_SUCCESS) {
            char const *msg = initResult == VRAPI_INITIALIZE_PERMISSIONS_ERROR ?
                              "Thread priority security exception. Make sure the APK is signed." :
                              "VrApi initialization error.";
            //SystemActivities_DisplayError(&java, SYSTEM_ACTIVITIES_FATAL_ERROR_OSIG, __FILE__, msg);
        }
        logger->log(LOG_DEBUG, "eglParams.createEGLContext()");
        this->eglParams->createEGLContext();
        this->glExtensions->eglInitExtensions();
        this->useMultiView &= (glExtensions->multiView &&
                               vrapi_GetSystemPropertyInt(&java,
                                                          VRAPI_SYS_PROP_MULTIVIEW_AVAILABLE));
        logger->log(LOG_DEBUG, "UseMultiview : " + this->useMultiView);

        this->perfParms = vrapi_DefaultPerformanceParms();
        perfParms.CpuLevel = CPU_LEVEL;
        perfParms.GpuLevel = GPU_LEVEL;
        perfParms.MainThreadTid = gettid();

        /*
         * Creating renderer specific config
         */
        this->renderer->create(&java, this->useMultiView);
        //renderer.create(&java, this-

        // Show a loading icon
        ovrFrameParms frameParms = vrapi_DefaultFrameParms(&this->java, VRAPI_FRAME_INIT_LOADING_ICON_FLUSH, vrapi_GetTimeInSeconds(), NULL );
        frameParms.FrameIndex = this->frameIndex;
        frameParms.PerformanceParms = perfParms;
        vrapi_SubmitFrame( this->ovrM, &frameParms);

    }

    bool RendererGearVR::initialize(Scene* scene) {
        IRenderable *sceneRenderer = scene->getRenderable();
        sceneRenderer->initialize();
        std::vector<Relation*> cameraRelations = scene->getRelations("camera");
        assert(cameraRelations.size() == 1);
        ((Camera*)cameraRelations[0])->getRenderable()->initialize();

        std::vector<Relation*> shaderRelations = scene->getRelations("shader");
        for (auto it = shaderRelations.cbegin(); it != shaderRelations.cend(); it++){
            Shader* shader = (Shader*)(*it);
            shader->getRenderable()->initialize();

            std::vector<Relation*> materialRelations = shader->getRelations("material");
            for (auto it = materialRelations.cbegin(); it != materialRelations.cend(); it++){
                Material *material = (Material*)(*it);
                material->getRenderable()->initialize();

                std::vector<Relation*> modelRelations = material->getRelations("model");
                for (auto it = modelRelations.cbegin(); it != modelRelations.cend(); it++){
                    Model *model = (Model*)(*it);
                    model->getRenderable()->initialize();
                }
            }
        }

        this->enterIntoVrMode();

        ovrFrameParms frameParms = vrapi_DefaultFrameParms( &this->java, VRAPI_FRAME_INIT_LOADING_ICON_FLUSH, vrapi_GetTimeInSeconds(), NULL );
        frameParms.FrameIndex = this->frameIndex;
        frameParms.PerformanceParms = perfParms;
        vrapi_SubmitFrame( this->ovrM, &frameParms);

        return true;
    }

    void RendererGearVR::update() {


    }

    void RendererGearVR::draw(Scene* scene) {
        // This is the only place the frame index is incremented, right before
        // calling vrapi_GetPredictedDisplayTime().
        this->frameIndex++;

        // Get the HMD pose, predicted for the middle of the time period during which
        // the new eye images will be displayed. The number of frames predicted ahead
        // depends on the pipeline depth of the engine and the synthesis rate.
        // The better the prediction, the less black will be pulled in at the edges.
        const double predictedDisplayTime = vrapi_GetPredictedDisplayTime(this->ovrM,
                                                                          this->frameIndex);
        const ovrTracking baseTracking = vrapi_GetPredictedTracking(this->ovrM,
                                                                    predictedDisplayTime);

        // Apply the head-on-a-stick model if there is no positional tracking.
        const ovrHeadModelParms headModelParms = vrapi_DefaultHeadModelParms();
        const ovrTracking tracking = vrapi_ApplyHeadModel(&headModelParms, &baseTracking);
        this->ovrSimulation->advance(predictedDisplayTime);

        // Render eye images and setup ovrFrameParms using ovrTracking.
        const ovrFrameParms frameParms = this->renderer->renderFrame(&this->java, this->frameIndex,
                                                                     this->minimumVSyncs, &this->perfParms, /*scene,*/
                                                                     this->ovrSimulation, scene,
                                                                     &tracking, this->ovrM);

        // Hand over the eye images to the time warp.
        vrapi_SubmitFrame( this->ovrM, &frameParms);
    }

    void RendererGearVR::deinitialize(Scene* scene) {
        IRenderable *sceneRenderer = scene->getRenderable();
        sceneRenderer->deinitialize();

        std::vector<Relation*> cameraRelations = scene->getRelations("camera");
        assert(cameraRelations.size() == 1);
        ((Camera*)cameraRelations[0])->getRenderable()->deinitialize();

        std::vector<Relation*> shaderRelations = scene->getRelations("shader");
        for (auto it = shaderRelations.cbegin(); it != shaderRelations.cend(); it++){
            Shader* shader = (Shader*)(*it);
            shader->getRenderable()->deinitialize();

            std::vector<Relation*> materialRelations = shader->getRelations("material");
            for (auto it = materialRelations.cbegin(); it != materialRelations.cend(); it++){
                Material *material = (Material*)(*it);
                material->getRenderable()->deinitialize();

                std::vector<Relation*> modelRelations = material->getRelations("model");
                for (auto it = modelRelations.cbegin(); it != modelRelations.cend(); it++){
                    Model *model = (Model*)(*it);
                    model->getRenderable()->deinitialize();
                }
            }
        }

        this->renderer->destroy();
        this->eglParams->destroyEGLContext();
        vrapi_Shutdown();

        //SystemActivities_Shutdown( &java );

        this->java.Vm->DetachCurrentThread();
    }

    void RendererGearVR::stop(){
        this->logger->log(LOG_INFO, "Stopping Window");
    }

    void RendererGearVR::setWindow(ANativeWindow *window) {
        this->window = window;
        this->logger->log(LOG_INFO, "Windows Set");
    }

    ANativeWindow* RendererGearVR::getWindow() {
        //this->window = window;
        this->logger->log(LOG_INFO, "Windows Get");
        return this->window;
    }

    void RendererGearVR::enterIntoVrMode() {
        if (this->ovrM == NULL) {
            ovrModeParms parms = vrapi_DefaultModeParms(&this->java);
            // Must reset the FLAG_FULLSCREEN window flag when using a SurfaceView
            parms.Flags |= VRAPI_MODE_FLAG_RESET_WINDOW_FULLSCREEN;

            parms.Flags |= VRAPI_MODE_FLAG_NATIVE_WINDOW;
            parms.Display = (size_t) this->eglParams->display;
            parms.WindowSurface = (size_t) this->window;
            parms.ShareContext = (size_t) this->eglParams->context;

            this->logger->log(LOG_DEBUG, "vrapi_EnterVrMode()");

            this->ovrM = vrapi_EnterVrMode(&parms);

            // If entering VR mode failed then the ANativeWindow was not valid.
            if (this->ovrM == NULL) {
                logger->log(LOG_ERROR, "Invalid ANativeWindow!");
                this->window = NULL;
            }
        }
    }

    void RendererGearVR::leaveVrMode() {
        logger->log(LOG_DEBUG, "vrapi_LeaveVrMode()");
        vrapi_LeaveVrMode(this->ovrM);
        this->ovrM = NULL;

    }

}