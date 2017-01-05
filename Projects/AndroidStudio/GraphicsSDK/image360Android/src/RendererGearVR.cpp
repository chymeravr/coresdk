//
// Created by robin_chimera on 1/2/2017.
//

//#include <assert.h>
#include <VrApi_Types.h>
#include <cassert>
#include <coreEngine/renderObjects/Camera.h>
#include <coreEngine/renderObjects/Shader.h>
#include <coreEngine/renderObjects/Material.h>
#include <coreEngine/renderObjects/Model.h>
#include <LoggerAndroid.h>
#include "VrApi_Types.h"
#include "../Include/RendererGearVR.h"

namespace cl {

#if defined EGL_SYNC
// EGL_KHR_reusable_sync
    PFNEGLCREATESYNCKHRPROC eglCreateSyncKHR;
    PFNEGLDESTROYSYNCKHRPROC eglDestroySyncKHR;
    PFNEGLCLIENTWAITSYNCKHRPROC eglClientWaitSyncKHR;
    PFNEGLSIGNALSYNCKHRPROC eglSignalSyncKHR;
    PFNEGLGETSYNCATTRIBKHRPROC eglGetSyncAttribKHR;
#endif


    /**
     *
     * RendererGearVR class methods
     *
     */

    RendererGearVR::RendererGearVR(JNIEnv *env, jobject activityObject)
            : logger(new LoggerAndroid("RendererGearVR")),
              window(NULL),
              ovrM(NULL),
              frameIndex(0),
              minimumVSyncs(1)
    {
        logger->log(LOG_INFO, "Renderer created");
        env->GetJavaVM(&this->javaVM);
        this->activityObject = env->NewGlobalRef(activityObject);
    }

    bool RendererGearVR::start() {
        java.Vm = this->javaVM;
        java.Vm->AttachCurrentThread(&java.Env, NULL);
        java.ActivityObject = this->activityObject;

        prctl(PR_SET_NAME, (long) "CL::Main", 0, 0, 0);
        logger->log(LOG_DEBUG, "SystemActivities_Init()");
        //SystemActivities_Init(&java);

        logger->log(LOG_DEBUG, "vrapi_DefaultInitParms()");
        const ovrInitParms initParms = vrapi_DefaultInitParms(&java);
        logger->log(LOG_DEBUG, "vrapi_Initialize()");
        int32_t initResult = vrapi_Initialize(&initParms);

        if (initResult != VRAPI_INITIALIZE_SUCCESS) {
            char const *msg = initResult == VRAPI_INITIALIZE_PERMISSIONS_ERROR ?
                              "Thread priority security exception. Make sure the APK is signed." :
                              "VrApi initialization error.";
            //SystemActivities_DisplayError(&java, SYSTEM_ACTIVITIES_FATAL_ERROR_OSIG, __FILE__, msg);
        }
        logger->log(LOG_DEBUG, "eglParams.createEGLContext()");
        eglParams.createEGLContext();
        this->glExtensions.eglInitExtensions();
        this->useMultiView &= (glExtensions.multiView &&
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
        renderer.create(&java, this->useMultiView);
    }

    bool RendererGearVR::initialize() {
        this->enterIntoVrMode();
    }

    void RendererGearVR::update() { }

    void RendererGearVR::draw() {
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
        ovrSimulation.advance(predictedDisplayTime);

        // Render eye images and setup ovrFrameParms using ovrTracking.
        const ovrFrameParms frameParms = this->renderer.renderFrame(&this->java, this->frameIndex, this->minimumVSyncs, &this->perfParms, /*scene,*/ &this->ovrSimulation, &tracking, this->ovrM);

        // Hand over the eye images to the time warp.
        vrapi_SubmitFrame( this->ovrM, &frameParms);
    }

    void RendererGearVR::destroy() {
        this->renderer.destroy();
        this->eglParams.destroyEGLContext();
        vrapi_Shutdown();

        //SystemActivities_Shutdown( &java );

        this->java.Vm->DetachCurrentThread();
    }

    void RendererGearVR::setWindow(ANativeWindow *window) {
        this->window = window;
        logger->log(LOG_INFO, "Windows Set");
    }

    ANativeWindow* RendererGearVR::getWindow() {
        //this->window = window;
        logger->log(LOG_INFO, "Windows Get");
        return this->window;
    }


    void RendererGearVR::enterIntoVrMode() {
        if (this->ovrM == NULL) {
            ovrModeParms parms = vrapi_DefaultModeParms(&this->java);
            // Must reset the FLAG_FULLSCREEN window flag when using a SurfaceView
            parms.Flags |= VRAPI_MODE_FLAG_RESET_WINDOW_FULLSCREEN;

            parms.Flags |= VRAPI_MODE_FLAG_NATIVE_WINDOW;
            parms.Display = (size_t) this->eglParams.display;
            parms.WindowSurface = (size_t) this->window;
            parms.ShareContext = (size_t) this->eglParams.context;

            logger->log(LOG_DEBUG, "vrapi_EnterVrMode()");

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

    /**
     *
     * OpenglExtensions_t class
     *
     */
    void RendererGearVR::OpenGLExtensions_t::eglInitExtensions() {
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


    /**
     *
     * EGLParams class methods
     *
     */

    RendererGearVR::EGLParams::EGLParams()
            //: logger(new LoggerAndroidImpl("RendererGearVR::EGLParams"))
    {
    }

    void RendererGearVR::EGLParams::clear() {
        this->majorVersion = 0;
        this->minorVersion = 0;
        this->display = 0;
        this->config = 0;
        this->tinySurface = EGL_NO_SURFACE;
        //this->mainsurface = EGL_NO_SURFACE;
        this->context = EGL_NO_CONTEXT;
    }

    void RendererGearVR::EGLParams::createEGLContext() {
        display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

        logger->log(LOG_DEBUG, "EGL initialized");
        eglInitialize(display, &majorVersion, &minorVersion);

        // Do NOT use eglChooseConfig, because the Android EGL code pushes in multisample
        // flags in eglChooseConfig if the user has selected the "force 4x MSAA" option in
        // settings, and that is completely wasted for our warp target.
        const int MAX_CONFIGS = 1024;
        EGLConfig configs[MAX_CONFIGS];
        EGLint numConfigs = 0;

        if (eglGetConfigs(display, configs, MAX_CONFIGS, &numConfigs) == EGL_FALSE) {
            logger->log(LOG_ERROR,
                        "eglGetConfigs() failed: " + this->getEglErrorString(glGetError()));
        }

        const EGLint configAttribs[] =
                {
                        EGL_RED_SIZE, 8,
                        EGL_GREEN_SIZE, 8,
                        EGL_BLUE_SIZE, 8,
                        EGL_ALPHA_SIZE, 8, // need alpha for the multi-pass timewarp compositor
                        EGL_DEPTH_SIZE, 0,
                        EGL_STENCIL_SIZE, 0,
                        EGL_SAMPLES, 0,
                        EGL_NONE
                };
        config = 0;

        for (int i = 0; i < numConfigs; i++) {
            EGLint value = 0;

            eglGetConfigAttrib(display, configs[i], EGL_RENDERABLE_TYPE, &value);
            if ((value & EGL_OPENGL_ES3_BIT_KHR) != EGL_OPENGL_ES3_BIT_KHR) {
                continue;
            }

            // The pbuffer config also needs to be compatible with normal window rendering
            // so it can share textures with the window context.
            eglGetConfigAttrib(display, configs[i], EGL_SURFACE_TYPE, &value);
            if ((value & (EGL_WINDOW_BIT | EGL_PBUFFER_BIT)) !=
                (EGL_WINDOW_BIT | EGL_PBUFFER_BIT)) {
                continue;
            }

            int j = 0;
            for (; configAttribs[j] != EGL_NONE; j += 2) {
                eglGetConfigAttrib(display, configs[i], configAttribs[j], &value);
                if (value != configAttribs[j + 1]) {
                    break;
                }
            }
            if (configAttribs[j] == EGL_NONE) {
                config = configs[i];
                break;
            }
        }
        if (config == 0) {
            logger->log(LOG_ERROR,
                        "eglChooseConfig() failed: " + this->getEglErrorString(eglGetError()));
            return;
        }
        EGLint contextAttribs[] =
                {
                        EGL_CONTEXT_CLIENT_VERSION, 3,
                        EGL_NONE
                };

        logger->log(LOG_DEBUG, "eglCreateContext");
        context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);

        if (context == EGL_NO_CONTEXT) {
            logger->log(LOG_ERROR,
                        "eglCreateContext() failed: " + this->getEglErrorString(eglGetError()));
            return;
        }

        const EGLint surfaceAttribs[] =
                {
                        EGL_WIDTH, 16,
                        EGL_HEIGHT, 16,
                        EGL_NONE
                };

        logger->log(LOG_DEBUG, "eglCreatePbufferSurface");
        tinySurface = eglCreatePbufferSurface(display, config, surfaceAttribs);

        if (tinySurface == EGL_NO_SURFACE) {
            logger->log(LOG_ERROR, "eglCreatePbufferSurface() failed: " +
                                   this->getEglErrorString(eglGetError()));
            eglDestroyContext(display, context);
            context = EGL_NO_CONTEXT;
            return;
        }

        logger->log(LOG_DEBUG, "eglMakeCurrent( Display, TinySurface, TinySurface, Context )");
        if (eglMakeCurrent(display, tinySurface, tinySurface, context) == EGL_FALSE) {
            logger->log(LOG_ERROR,
                        "eglMakeCurrent() failed: " + this->getEglErrorString(eglGetError()));
            eglDestroySurface(display, tinySurface);
            eglDestroyContext(display, context);
            context = EGL_NO_CONTEXT;
            return;
        }
    }

    void RendererGearVR::EGLParams::destroyEGLContext() {
        if (display != 0) {
            logger->log(LOG_DEBUG,
                        "eglMakeCurrent (display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT)");
            if (eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT) ==
                EGL_FALSE) {
                logger->log(LOG_ERROR,
                            "eglMakeCurrent failed: " + getEglErrorString(eglGetError()));
            }
        }
        if (context != EGL_NO_CONTEXT) {
            logger->log(LOG_DEBUG, "eglDestroyContext(display, context)");
            if (eglDestroyContext(display, context) == EGL_FALSE) {
                logger->log(LOG_ERROR, "eglDestroyContext(display, context) failed: " +
                                       getEglErrorString(eglGetError()));
            }
            context = EGL_NO_CONTEXT;
        }
        if (tinySurface != EGL_NO_SURFACE) {
            logger->log(LOG_DEBUG, "eglDestroySurface(display, surface)");
            if (eglDestroySurface(display, surface) == EGL_FALSE) {
                logger->log(LOG_ERROR, "eglDestroySurface(display, surface) failed: " +
                                       getEglErrorString(eglGetError()));
            }
            tinySurface = EGL_NO_SURFACE;
        }
        if (display != 0) {
            logger->log(LOG_DEBUG, "eglTerminate(display)");
            if (eglTerminate(display) == EGL_FALSE) {
                logger->log(LOG_ERROR,
                            "eglTerminate(display) failed:" + getEglErrorString(eglGetError()));
            }
            display = 0;
        }
    }

    std::string RendererGearVR::EGLParams::getEglErrorString(const EGLint error) {
        switch (error) {
            case EGL_SUCCESS:
                return "EGL_SUCCESS";
            case EGL_NOT_INITIALIZED:
                return "EGL_NOT_INITIALIZED";
            case EGL_BAD_ACCESS:
                return "EGL_BAD_ACCESS";
            case EGL_BAD_ALLOC:
                return "EGL_BAD_ALLOC";
            case EGL_BAD_ATTRIBUTE:
                return "EGL_BAD_ATTRIBUTE";
            case EGL_BAD_CONTEXT:
                return "EGL_BAD_CONTEXT";
            case EGL_BAD_CONFIG:
                return "EGL_BAD_CONFIG";
            case EGL_BAD_CURRENT_SURFACE:
                return "EGL_BAD_CURRENT_SURFACE";
            case EGL_BAD_DISPLAY:
                return "EGL_BAD_DISPLAY";
            case EGL_BAD_SURFACE:
                return "EGL_BAD_SURFACE";
            case EGL_BAD_MATCH:
                return "EGL_BAD_MATCH";
            case EGL_BAD_PARAMETER:
                return "EGL_BAD_PARAMETER";
            case EGL_BAD_NATIVE_PIXMAP:
                return "EGL_BAD_NATIVE_PIXMAP";
            case EGL_BAD_NATIVE_WINDOW:
                return "EGL_BAD_NATIVE_WINDOW";
            case EGL_CONTEXT_LOST:
                return "EGL_CONTEXT_LOST";
            default:
                return "unknown";
        }
    }

    /**
     *
     * OvrFrameRendere class methods
     *
     */
    RendererGearVR::OvrFrameBuffer::OvrFrameBuffer()
             : logger(new LoggerAndroid("RendererGearVR::OvrFrameBuffer"))
    {
    }

    void RendererGearVR::OvrFrameBuffer::clear() {
        this->width = 0;
        this->height = 0;
        this->multisamples = 0;
        this->textureSwapChainLength = 0;
        this->textureSwapChainIndex = 0;
        this->useMultiView = false;
        this->colorTextureSwapChain = NULL;
        this->depthBuffers = NULL;
        this->frameBuffers = NULL;
    }

    bool RendererGearVR::OvrFrameBuffer::create(const bool useMultiview,
                                                const ovrTextureFormat colorFormat, const int width,
                                                const int height, const int multisamples) {
        PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC glRenderbufferStorageMultisampleEXT =
                (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC) eglGetProcAddress(
                        "glRenderbufferStorageMultisampleEXT");
        PFNGLFRAMEBUFFERTEXTURE2DMULTISAMPLEEXTPROC glFramebufferTexture2DMultisampleEXT =
                (PFNGLFRAMEBUFFERTEXTURE2DMULTISAMPLEEXTPROC) eglGetProcAddress(
                        "glFramebufferTexture2DMultisampleEXT");

        PFNGLFRAMEBUFFERTEXTUREMULTIVIEWOVRPROC glFramebufferTextureMultiviewOVR =
                (PFNGLFRAMEBUFFERTEXTUREMULTIVIEWOVRPROC) eglGetProcAddress(
                        "glFramebufferTextureMultiviewOVR");
        PFNGLFRAMEBUFFERTEXTUREMULTISAMPLEMULTIVIEWOVRPROC glFramebufferTextureMultisampleMultiviewOVR =
                (PFNGLFRAMEBUFFERTEXTUREMULTISAMPLEMULTIVIEWOVRPROC) eglGetProcAddress(
                        "glFramebufferTextureMultisampleMultiviewOVR");

        this->width = width;
        this->height = height;
        this->multisamples = multisamples;
        this->useMultiView = useMultiview;

        this->colorTextureSwapChain = vrapi_CreateTextureSwapChain(
                this->useMultiView ? VRAPI_TEXTURE_TYPE_2D_ARRAY : VRAPI_TEXTURE_TYPE_2D,
                colorFormat, this->width, this->height, 1, true);
        this->textureSwapChainLength = vrapi_GetTextureSwapChainLength(this->colorTextureSwapChain);
        this->depthBuffers = (GLuint *) malloc(this->textureSwapChainLength * sizeof(GLuint));
        this->frameBuffers = (GLuint *) malloc(this->textureSwapChainLength * sizeof(GLuint));

        logger->log(LOG_DEBUG, "OvrFrameBuffer->useMultiView:" + this->useMultiView);

        for (int i = 0; i < this->textureSwapChainLength; i++) {
            //create color buffer texture
            const GLuint colorTexture = vrapi_GetTextureSwapChainHandle(this->colorTextureSwapChain,
                                                                        i);
            GLenum colorTextureTarget = this->useMultiView ? GL_TEXTURE_2D_ARRAY : GL_TEXTURE_2D;
            GL(glBindTexture(colorTextureTarget, colorTexture));
            GL(glTexParameteri(colorTextureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
            GL(glTexParameteri(colorTextureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
            GL(glTexParameteri(colorTextureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            GL(glTexParameteri(colorTextureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            GL(glBindTexture(colorTextureTarget, 0));

            if (this->useMultiView) {
                //Create depth buffer texture
                GL(glGenTextures(1, &this->depthBuffers[i]));
                GL(glBindTexture(GL_TEXTURE_2D_ARRAY, this->depthBuffers[i]));
                GL(glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_DEPTH_COMPONENT24, this->width,
                                  this->height, 2));
                GL(glBindTexture(GL_TEXTURE_2D_ARRAY, 0));

                //create Frame buffer
                GL(glGenFramebuffers(1, &this->frameBuffers[i]));
                GL(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->frameBuffers[i]));
                if (this->multisamples > 1 &&
                    (glFramebufferTextureMultisampleMultiviewOVR != NULL)) {
                    GL(glFramebufferTextureMultisampleMultiviewOVR(GL_DRAW_FRAMEBUFFER,
                                                                   GL_DEPTH_ATTACHMENT,
                                                                   this->depthBuffers[i],
                                                                   0 /* level */,
                                                                   this->multisamples /* samples */,
                                                                   0 /* baseViewIndex */,
                                                                   2 /* numViews */ ))
                    GL(glFramebufferTextureMultisampleMultiviewOVR(GL_DRAW_FRAMEBUFFER,
                                                                   GL_COLOR_ATTACHMENT0,
                                                                   colorTexture, 0 /* level */,
                                                                   multisamples /* samples */,
                                                                   0 /* baseViewIndex */,
                                                                   2 /* numViews */ ));
                } else {
                    GL(glFramebufferTextureMultiviewOVR(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                                        this->depthBuffers[i], 0 /* level */,
                                                        0 /* baseViewIndex */, 2 /* numViews */ ));
                    GL(glFramebufferTextureMultiviewOVR(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                                        colorTexture, 0 /* level */,
                                                        0 /* baseViewIndex */, 2 /* numViews */ ));
                }
                GL(GLenum renderFrameBufferStatus = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER));
                GL(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
                if (renderFrameBufferStatus != GL_FRAMEBUFFER_COMPLETE) {
                    logger->log(LOG_ERROR, "Incomplete frame buffer object: " +
                                           frameBufferStatusString(renderFrameBufferStatus));
                    return false;
                }

            } else { //useMultiView
                if (this->multisamples > 1 && glRenderbufferStorageMultisampleEXT != NULL &&
                    glFramebufferTexture2DMultisampleEXT != NULL) {
                    //create multisample depth buffer
                    GL(glGenRenderbuffers(1, &this->depthBuffers[i]));
                    GL(glBindRenderbuffer(GL_RENDERBUFFER, this->depthBuffers[i]));
                    GL(glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER, this->multisamples,
                                                           GL_DEPTH_COMPONENT24, width, height));
                    GL(glBindRenderbuffer(GL_RENDERBUFFER, 0));

                    //create framebuffer
                    GL(glGenFramebuffers(1, &this->frameBuffers[i]));
                    GL(glBindFramebuffer(GL_FRAMEBUFFER, this->frameBuffers[i]));
                    GL(glFramebufferTexture2DMultisampleEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                                            GL_TEXTURE_2D, colorTexture, 0,
                                                            this->multisamples));
                    GL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                                 GL_RENDERBUFFER, this->depthBuffers[i]));
                    GL(GLenum renderFrameBufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER));
                    GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
                    if (renderFrameBufferStatus != GL_FRAMEBUFFER_COMPLETE) {
                        logger->log(LOG_ERROR, "Incomplete frame buffer object:" +
                                               frameBufferStatusString(renderFrameBufferStatus));
                        return false;
                    }
                } else {
                    //create depth buffers
                    GL(glGenRenderbuffers(1, &this->depthBuffers[i]));
                    GL(glBindRenderbuffer(GL_RENDERBUFFER, this->depthBuffers[i]));
                    GL(glRenderbufferStorage(GL_FRAMEBUFFER, GL_DEPTH_COMPONENT24, width, height));
                    GL(glBindFramebuffer(GL_RENDERBUFFER, 0));

                    //create framebuffer
                    GL(glGenFramebuffers(1, &this->frameBuffers[i]));
                    GL(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->frameBuffers[i]));
                    GL(glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                                 GL_RENDERBUFFER, this->frameBuffers[i]));
                    GL(glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                              GL_TEXTURE_2D, colorTexture, 0));
                    GL(GLenum renderFrameBufferStatus = glCheckFramebufferStatus(
                               GL_DRAW_FRAMEBUFFER));
                    GL(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
                    if (renderFrameBufferStatus != GL_FRAMEBUFFER_COMPLETE) {
                        logger->log(LOG_ERROR, "Incomplete frame buffer object:" +
                                               frameBufferStatusString(renderFrameBufferStatus));
                        return false;
                    }
                }
            }

        }
        return true;
    }

    void RendererGearVR::OvrFrameBuffer::destroy() {
        GL(glDeleteFramebuffers(this->textureSwapChainLength, this->frameBuffers));
        if (this->useMultiView) {
            GL(glDeleteTextures(this->textureSwapChainLength, this->depthBuffers));
        }
        else {
            GL(glDeleteRenderbuffers(this->textureSwapChainLength, this->depthBuffers));
        }
        vrapi_DestroyTextureSwapChain(this->colorTextureSwapChain);

        free(this->depthBuffers);
        free(this->frameBuffers);

        this->clear();
    }

    void RendererGearVR::OvrFrameBuffer::setCurrent() {
        GL(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->frameBuffers[this->textureSwapChainIndex]));
    }

    void RendererGearVR::OvrFrameBuffer::setNone() {
        GL(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
    }

    void RendererGearVR::OvrFrameBuffer::resolve() {
        const GLenum depthAttachment[1] = {GL_DEPTH_ATTACHMENT};
        glInvalidateFramebuffer(GL_DRAW_FRAMEBUFFER, 1, depthAttachment);

        //Flush this frame worth of commands
        glFlush();
    }

    void RendererGearVR::OvrFrameBuffer::advance() {
        this->textureSwapChainIndex =
                (this->textureSwapChainIndex + 1) % this->textureSwapChainLength;
    }

    std::string RendererGearVR::OvrFrameBuffer::frameBufferStatusString(GLenum error) {
        switch ( error )
        {
            case GL_FRAMEBUFFER_UNDEFINED:						return "GL_FRAMEBUFFER_UNDEFINED";
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:			return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:	return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
            case GL_FRAMEBUFFER_UNSUPPORTED:					return "GL_FRAMEBUFFER_UNSUPPORTED";
            case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:			return "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
            default:											return "unknown";
        }
    }

    /**
     *
     * OvrRenderer class methods
     *
     */
    RendererGearVR::OvrRenderer::OvrRenderer()
             : logger(new LoggerAndroid("RendererGearVR::OvrRenderer"))
    {
    }

    void RendererGearVR::OvrRenderer::clear() {

        for (int eye = 0; eye < VRAPI_FRAME_LAYER_EYE_MAX; eye++) {
            this->clear();
            //ovrFramebuffer_Clear( &renderer->FrameBuffer[eye] );
        }
        this->projectionMatrix = ovrMatrix4f_CreateIdentity();
        this->texCoordsTanAnglesMatrix = ovrMatrix4f_CreateIdentity();
        this->numBuffers = VRAPI_FRAME_LAYER_EYE_MAX;
    }

    void RendererGearVR::OvrRenderer::create(const ovrJava *java, const bool useMultiview) {
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

    void RendererGearVR::OvrRenderer::destroy() {
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

    ovrFrameParms RendererGearVR::OvrRenderer::renderFrame(const ovrJava *java,
                                                           long long frameIndex, int minimumVsyncs,
                                                           const ovrPerformanceParms *perfParms,
                                                           const OvrSimulation *simulation,
                                                           const ovrTracking *tracking,
                                                           ovrMobile *ovr) {
        ovrFrameParms parms = vrapi_DefaultFrameParms(java, VRAPI_FRAME_INIT_DEFAULT, vrapi_GetTimeInSeconds(), NULL);
        parms.FrameIndex = frameIndex;
        parms.MinimumVsyncs = minimumVsyncs;
        parms.PerformanceParms = *perfParms;
        parms.Layers[VRAPI_FRAME_LAYER_TYPE_WORLD].Flags |= VRAPI_FRAME_LAYER_FLAG_CHROMATIC_ABERRATION_CORRECTION;

        const ovrHeadModelParms headModelParms = vrapi_DefaultHeadModelParms();

#if REDUCED_LATENCY
        ovrTracking updatedTracking = vrapi_GetPredictedTracking(ovr, tracking->HeadPose.TimeInSeconds);
        updatedTracking.HeadPose.Pose.Position = tracking->HeadPose.Pose.Position;
#else
        ovrTracking updatedTracking = *tracking;
#endif

        // Calculate the view matrix.
        const ovrMatrix4f centerEyeViewMatrix = vrapi_GetCenterEyeViewMatrix( &headModelParms, &updatedTracking, NULL );

        ovrMatrix4f eyeViewMatrix[2];
        eyeViewMatrix[0] = vrapi_GetEyeViewMatrix( &headModelParms, &centerEyeViewMatrix, 0 );
        eyeViewMatrix[1] = vrapi_GetEyeViewMatrix( &headModelParms, &centerEyeViewMatrix, 1 );

        ovrMatrix4f eyeViewMatrixTransposed[2];
        eyeViewMatrixTransposed[0] = ovrMatrix4f_Transpose( &eyeViewMatrix[0] );
        eyeViewMatrixTransposed[1] = ovrMatrix4f_Transpose( &eyeViewMatrix[1] );

        ovrMatrix4f projectionMatrixTransposed;
        projectionMatrixTransposed = ovrMatrix4f_Transpose(&this->projectionMatrix);

        for(int eye=0; eye < VRAPI_FRAME_LAYER_EYE_MAX; eye++){
            OvrFrameBuffer *frameBuffer = &this->frameBuffer[this->numBuffers == 1 ? 0 : eye];
            parms.Layers[VRAPI_FRAME_LAYER_TYPE_WORLD].Textures[eye].ColorTextureSwapChain = frameBuffer->colorTextureSwapChain;
            parms.Layers[VRAPI_FRAME_LAYER_TYPE_WORLD].Textures[eye].TextureSwapChainIndex = frameBuffer->textureSwapChainIndex;
            parms.Layers[VRAPI_FRAME_LAYER_TYPE_WORLD].Textures[eye].TexCoordsFromTanAngles = this->texCoordsTanAnglesMatrix;
            parms.Layers[VRAPI_FRAME_LAYER_TYPE_WORLD].Textures[eye].HeadPose = updatedTracking.HeadPose;
        }

        unsigned long long completionFence[VRAPI_FRAME_LAYER_EYE_MAX] = { 0 };

        for(int eye=0; eye<this->numBuffers; eye++){
            // NOTE: In the non-mv case, latency can be further reduced by updating the sensor prediction
            // for each eye (updates orientation, not position)
            OvrFrameBuffer * frameBuffer = &this->frameBuffer[eye];
            frameBuffer->setCurrent();
            GL( glEnable( GL_SCISSOR_TEST ) );
            GL( glDepthMask( GL_TRUE ) );
            GL( glEnable( GL_DEPTH_TEST ) );
            GL( glDepthFunc( GL_LEQUAL ) );
            GL( glEnable( GL_CULL_FACE ) );
            GL( glCullFace( GL_BACK ) );
            GL( glViewport( 0, 0, frameBuffer->width, frameBuffer->height ) );
            GL( glScissor( 0, 0, frameBuffer->width, frameBuffer->height ) );
            GL( glClearColor( 0.125f, 0.0f, 0.125f, 1.0f ) );
            GL( glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) );
            //GL( glBindVertexArray( scene->Cube.VertexArrayObject ) );
            //GL( glDrawElementsInstanced( GL_TRIANGLES, scene->Cube.IndexCount, GL_UNSIGNED_SHORT, NULL, NUM_INSTANCES ) );
            //GL( glBindVertexArray( 0 ) );


            // Explicitly clear the border texels to black because OpenGL-ES does not support GL_CLAMP_TO_BORDER.
            {
                // Clear to fully opaque black.
                GL( glClearColor( 0.0f, 0.0f, 0.0f, 1.0f ) );
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
            OvrFence * fence = &this->fence[eye][frameBuffer->textureSwapChainIndex];
            fence->insert();
            completionFence[eye] = (size_t)fence->sync;

            frameBuffer->advance();
        }
        for ( int eye = 0; eye < VRAPI_FRAME_LAYER_EYE_MAX; eye++ )
        {
            parms.Layers[VRAPI_FRAME_LAYER_TYPE_WORLD].Textures[eye].CompletionFence =
                    completionFence[this->numBuffers == 1 ? 0 : eye];
        }
        OvrFrameBuffer::setNone();

        return parms;
    }

    /**
     *
     * OvrFence class methods
     *
     */
    void RendererGearVR::OvrFence::create() {
#if defined( EGL_SYNC )
        this->display = 0;
        this->sync = EGL_NO_SYNC_KHR;
#else
        this->sync = 0;
#endif
    }

    void RendererGearVR::OvrFence::destroy() {
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
        if ( this->sync != 0 )
    {
        glDeleteSync( this->sync );
        this->sync = 0;
    }
#endif
    }

    void RendererGearVR::OvrFence::insert() {
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
    this->sync = glFenceSync( GL_SYNC_GPU_COMMANDS_COMPLETE, 0 );
    // Force flushing the commands.
    // Note that some drivers will already flush when calling glFenceSync.
    glClientWaitSync( this->sync, GL_SYNC_FLUSH_COMMANDS_BIT, 0 );
#endif

    }

    /**
     *
     * OvrSimulation class methods
     *
     */
    void RendererGearVR::OvrSimulation::clear() {
        this->currentRotation.x = 0.0f;
        this->currentRotation.y = 0.0f;
        this->currentRotation.z = 0.0f;
    }

    void RendererGearVR::OvrSimulation::advance(double predictedDisplayTime) {
        this->currentRotation.x = (float) (predictedDisplayTime);
        this->currentRotation.y = (float) (predictedDisplayTime);
        this->currentRotation.z = (float) (predictedDisplayTime);
    }
}