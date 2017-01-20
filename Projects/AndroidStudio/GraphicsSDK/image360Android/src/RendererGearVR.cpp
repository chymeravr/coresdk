//
// Created by robin_chimera on 1/9/2017.
//

#include <VrApi_Types.h>
#include <coreEngine/renderObjects/Shader.h>
#include <coreEngine/renderObjects/Material.h>
#include <coreEngine/renderObjects/Model.h>
#include <CameraGLOVR.h>
#include <RendererGearVR.h>

namespace cl {

#if defined EGL_SYNC
// EGL_KHR_reusable_sync
    PFNEGLCREATESYNCKHRPROC eglCreateSyncKHR;
    PFNEGLDESTROYSYNCKHRPROC eglDestroySyncKHR;
    PFNEGLCLIENTWAITSYNCKHRPROC eglClientWaitSyncKHR;
    PFNEGLSIGNALSYNCKHRPROC eglSignalSyncKHR;
    PFNEGLGETSYNCATTRIBKHRPROC eglGetSyncAttribKHR;
#endif



    static void EglInitExtensions( OpenGLExtensions_t *glExtensions)
    {
#if defined EGL_SYNC
        eglCreateSyncKHR		= (PFNEGLCREATESYNCKHRPROC)			eglGetProcAddress( "eglCreateSyncKHR" );
        eglDestroySyncKHR		= (PFNEGLDESTROYSYNCKHRPROC)		eglGetProcAddress( "eglDestroySyncKHR" );
        eglClientWaitSyncKHR	= (PFNEGLCLIENTWAITSYNCKHRPROC)		eglGetProcAddress( "eglClientWaitSyncKHR" );
        eglSignalSyncKHR		= (PFNEGLSIGNALSYNCKHRPROC)			eglGetProcAddress( "eglSignalSyncKHR" );
        eglGetSyncAttribKHR		= (PFNEGLGETSYNCATTRIBKHRPROC)		eglGetProcAddress( "eglGetSyncAttribKHR" );
#endif

        // get extension pointers
        const char * allExtensions = (const char *)glGetString( GL_EXTENSIONS );
        if ( allExtensions != NULL )
        {
            glExtensions->multi_view = strstr( allExtensions, "GL_OVR_multiview2" ) &&
                                      strstr( allExtensions, "GL_OVR_multiview_multisampled_render_to_texture" );
        }
    }


    /**
     *
     * EGLParams methods
     *
     */

    static void ovrEgl_Clear( ovrEgl * egl )
    {
        egl->MajorVersion = 0;
        egl->MinorVersion = 0;
        egl->Display = 0;
        egl->Config = 0;
        egl->TinySurface = EGL_NO_SURFACE;
        egl->MainSurface = EGL_NO_SURFACE;
        egl->Context = EGL_NO_CONTEXT;
    }

    static void ovrEgl_CreateContext( ovrEgl * egl, const ovrEgl * shareEgl )
    {
        if ( egl->Display != 0 )
        {
            return;
        }

        egl->Display = eglGetDisplay( EGL_DEFAULT_DISPLAY );
        ALOGV( "        eglInitialize( Display, &MajorVersion, &MinorVersion )" );
        eglInitialize( egl->Display, &egl->MajorVersion, &egl->MinorVersion );
        // Do NOT use eglChooseConfig, because the Android EGL code pushes in multisample
        // flags in eglChooseConfig if the user has selected the "force 4x MSAA" option in
        // settings, and that is completely wasted for our warp target.
        const int MAX_CONFIGS = 1024;
        EGLConfig configs[MAX_CONFIGS];
        EGLint numConfigs = 0;
        if ( eglGetConfigs( egl->Display, configs, MAX_CONFIGS, &numConfigs ) == EGL_FALSE )
        {
            ALOGE( "        eglGetConfigs() failed: %s", EglErrorString( eglGetError() ) );
            return;
        }
        const EGLint configAttribs[] =
                {
                        EGL_RED_SIZE,		8,
                        EGL_GREEN_SIZE,		8,
                        EGL_BLUE_SIZE,		8,
                        EGL_ALPHA_SIZE,		8, // need alpha for the multi-pass timewarp compositor
                        EGL_DEPTH_SIZE,		0,
                        EGL_STENCIL_SIZE,	0,
                        EGL_SAMPLES,		0,
                        EGL_NONE
                };
        egl->Config = 0;
        for ( int i = 0; i < numConfigs; i++ )
        {
            EGLint value = 0;

            eglGetConfigAttrib( egl->Display, configs[i], EGL_RENDERABLE_TYPE, &value );
            if ( ( value & EGL_OPENGL_ES3_BIT_KHR ) != EGL_OPENGL_ES3_BIT_KHR )
            {
                continue;
            }

            // The pbuffer config also needs to be compatible with normal window rendering
            // so it can share textures with the window context.
            eglGetConfigAttrib( egl->Display, configs[i], EGL_SURFACE_TYPE, &value );
            if ( ( value & ( EGL_WINDOW_BIT | EGL_PBUFFER_BIT ) ) != ( EGL_WINDOW_BIT | EGL_PBUFFER_BIT ) )
            {
                continue;
            }

            int	j = 0;
            for ( ; configAttribs[j] != EGL_NONE; j += 2 )
            {
                eglGetConfigAttrib( egl->Display, configs[i], configAttribs[j], &value );
                if ( value != configAttribs[j + 1] )
                {
                    break;
                }
            }
            if ( configAttribs[j] == EGL_NONE )
            {
                egl->Config = configs[i];
                break;
            }
        }
        if ( egl->Config == 0 )
        {
            ALOGE( "        eglChooseConfig() failed: %s", EglErrorString( eglGetError() ) );
            return;
        }
        EGLint contextAttribs[] =
                {
                        EGL_CONTEXT_CLIENT_VERSION, 3,
                        EGL_NONE
                };
        ALOGV( "        Context = eglCreateContext( Display, Config, EGL_NO_CONTEXT, contextAttribs )" );
        egl->Context = eglCreateContext( egl->Display, egl->Config, ( shareEgl != NULL ) ? shareEgl->Context : EGL_NO_CONTEXT, contextAttribs );
        if ( egl->Context == EGL_NO_CONTEXT )
        {
            ALOGE( "        eglCreateContext() failed: %s", EglErrorString( eglGetError() ) );
            return;
        }
        const EGLint surfaceAttribs[] =
                {
                        EGL_WIDTH, 16,
                        EGL_HEIGHT, 16,
                        EGL_NONE
                };
        ALOGV( "        TinySurface = eglCreatePbufferSurface( Display, Config, surfaceAttribs )" );
        egl->TinySurface = eglCreatePbufferSurface( egl->Display, egl->Config, surfaceAttribs );
        if ( egl->TinySurface == EGL_NO_SURFACE )
        {
            ALOGE( "        eglCreatePbufferSurface() failed: %s", EglErrorString( eglGetError() ) );
            eglDestroyContext( egl->Display, egl->Context );
            egl->Context = EGL_NO_CONTEXT;
            return;
        }
        ALOGV( "        eglMakeCurrent( Display, TinySurface, TinySurface, Context )" );
        if ( eglMakeCurrent( egl->Display, egl->TinySurface, egl->TinySurface, egl->Context ) == EGL_FALSE )
        {
            ALOGE( "        eglMakeCurrent() failed: %s", EglErrorString( eglGetError() ) );
            eglDestroySurface( egl->Display, egl->TinySurface );
            eglDestroyContext( egl->Display, egl->Context );
            egl->Context = EGL_NO_CONTEXT;
            return;
        }
    }

    static void ovrEgl_DestroyContext( ovrEgl * egl )
    {
        if ( egl->Display != 0 )
        {
            ALOGE( "        eglMakeCurrent( Display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT )" );
            if ( eglMakeCurrent( egl->Display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT ) == EGL_FALSE )
            {
                ALOGE( "        eglMakeCurrent() failed: %s", EglErrorString( eglGetError() ) );
            }
        }
        if ( egl->Context != EGL_NO_CONTEXT )
        {
            ALOGE( "        eglDestroyContext( Display, Context )" );
            if ( eglDestroyContext( egl->Display, egl->Context ) == EGL_FALSE )
            {
                ALOGE( "        eglDestroyContext() failed: %s", EglErrorString( eglGetError() ) );
            }
            egl->Context = EGL_NO_CONTEXT;
        }
        if ( egl->TinySurface != EGL_NO_SURFACE )
        {
            ALOGE( "        eglDestroySurface( Display, TinySurface )" );
            if ( eglDestroySurface( egl->Display, egl->TinySurface ) == EGL_FALSE )
            {
                ALOGE( "        eglDestroySurface() failed: %s", EglErrorString( eglGetError() ) );
            }
            egl->TinySurface = EGL_NO_SURFACE;
        }
        if ( egl->Display != 0 )
        {
            ALOGE( "        eglTerminate( Display )" );
            if ( eglTerminate( egl->Display ) == EGL_FALSE )
            {
                ALOGE( "        eglTerminate() failed: %s", EglErrorString( eglGetError() ) );
            }
            egl->Display = 0;
        }
    }

    /**
     *
     * OvrFrameBuffer methods
     *
     */
    static void ovrFramebuffer_Clear( ovrFramebuffer * frameBuffer )
    {
        frameBuffer->Width = 0;
        frameBuffer->Height = 0;
        frameBuffer->Multisamples = 0;
        frameBuffer->TextureSwapChainLength = 0;
        frameBuffer->TextureSwapChainIndex = 0;
        frameBuffer->UseMultiview = false;
        frameBuffer->ColorTextureSwapChain = NULL;
        frameBuffer->DepthBuffers = NULL;
        frameBuffer->FrameBuffers = NULL;
    }

    static bool ovrFramebuffer_Create( ovrFramebuffer * frameBuffer, const bool useMultiview,
                                       const ovrTextureFormat colorFormat, const int width,
                                       const int height, const int multisamples )
    {
        PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC glRenderbufferStorageMultisampleEXT =
                (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC)eglGetProcAddress( "glRenderbufferStorageMultisampleEXT" );
        PFNGLFRAMEBUFFERTEXTURE2DMULTISAMPLEEXTPROC glFramebufferTexture2DMultisampleEXT =
                (PFNGLFRAMEBUFFERTEXTURE2DMULTISAMPLEEXTPROC)eglGetProcAddress( "glFramebufferTexture2DMultisampleEXT" );

        PFNGLFRAMEBUFFERTEXTUREMULTIVIEWOVRPROC glFramebufferTextureMultiviewOVR =
                (PFNGLFRAMEBUFFERTEXTUREMULTIVIEWOVRPROC) eglGetProcAddress( "glFramebufferTextureMultiviewOVR" );
        PFNGLFRAMEBUFFERTEXTUREMULTISAMPLEMULTIVIEWOVRPROC glFramebufferTextureMultisampleMultiviewOVR =
                (PFNGLFRAMEBUFFERTEXTUREMULTISAMPLEMULTIVIEWOVRPROC) eglGetProcAddress( "glFramebufferTextureMultisampleMultiviewOVR" );

        frameBuffer->Width = width;
        frameBuffer->Height = height;
        frameBuffer->Multisamples = multisamples;
        frameBuffer->UseMultiview = ( useMultiview && ( glFramebufferTextureMultiviewOVR != NULL ) ) ? true : false;

        frameBuffer->ColorTextureSwapChain = vrapi_CreateTextureSwapChain( frameBuffer->UseMultiview ? VRAPI_TEXTURE_TYPE_2D_ARRAY : VRAPI_TEXTURE_TYPE_2D,
                                                                           colorFormat, width, height, 1, true );
        frameBuffer->TextureSwapChainLength = vrapi_GetTextureSwapChainLength( frameBuffer->ColorTextureSwapChain );
        frameBuffer->DepthBuffers = (GLuint *)malloc( frameBuffer->TextureSwapChainLength * sizeof( GLuint ) );
        frameBuffer->FrameBuffers = (GLuint *)malloc( frameBuffer->TextureSwapChainLength * sizeof( GLuint ) );

        ALOGV( "        frameBuffer->UseMultiview = %d", frameBuffer->UseMultiview );

        for ( int i = 0; i < frameBuffer->TextureSwapChainLength; i++ )
        {
            // Create the color buffer texture.
            const GLuint colorTexture = vrapi_GetTextureSwapChainHandle( frameBuffer->ColorTextureSwapChain, i );
            GLenum colorTextureTarget = frameBuffer->UseMultiview ? GL_TEXTURE_2D_ARRAY : GL_TEXTURE_2D;
            GL( glBindTexture( colorTextureTarget, colorTexture ) );
            GL( glTexParameteri( colorTextureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ) );
            GL( glTexParameteri( colorTextureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ) );
            GL( glTexParameteri( colorTextureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR ) );
            GL( glTexParameteri( colorTextureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR ) );
            GL( glBindTexture( colorTextureTarget, 0 ) );

            if ( frameBuffer->UseMultiview )
            {
                // Create the depth buffer texture.
                GL( glGenTextures( 1, &frameBuffer->DepthBuffers[i] ) );
                GL( glBindTexture( GL_TEXTURE_2D_ARRAY, frameBuffer->DepthBuffers[i] ) );
                GL( glTexStorage3D( GL_TEXTURE_2D_ARRAY, 1, GL_DEPTH_COMPONENT24, width, height, 2 ) );
                GL( glBindTexture( GL_TEXTURE_2D_ARRAY, 0 ) );

                // Create the frame buffer.
                GL( glGenFramebuffers( 1, &frameBuffer->FrameBuffers[i] ) );
                GL( glBindFramebuffer( GL_DRAW_FRAMEBUFFER, frameBuffer->FrameBuffers[i] ) );
                if ( multisamples > 1 && ( glFramebufferTextureMultisampleMultiviewOVR != NULL ) )
                {
                    GL( glFramebufferTextureMultisampleMultiviewOVR( GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                                                     frameBuffer->DepthBuffers[i], 0 /* level */,
                                                                     multisamples /* samples */, 0 /* baseViewIndex */, 2 /* numViews */ ) );
                    GL( glFramebufferTextureMultisampleMultiviewOVR( GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                                                     colorTexture, 0 /* level */, multisamples /* samples */,
                                                                     0 /* baseViewIndex */, 2 /* numViews */ ) );
                }
                else
                {
                    GL( glFramebufferTextureMultiviewOVR( GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, frameBuffer->DepthBuffers[i],
                                                          0 /* level */, 0 /* baseViewIndex */, 2 /* numViews */ ) );
                    GL( glFramebufferTextureMultiviewOVR( GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colorTexture, 0 /* level */, 0 /* baseViewIndex */, 2 /* numViews */ ) );
                }

                GL( GLenum renderFramebufferStatus = glCheckFramebufferStatus( GL_DRAW_FRAMEBUFFER ) );
                GL( glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 ) );
                if ( renderFramebufferStatus != GL_FRAMEBUFFER_COMPLETE )
                {
                    ALOGE( "Incomplete frame buffer object: %s", GlFrameBufferStatusString( renderFramebufferStatus ) );
                    return false;
                }
            }
            else
            {
                if ( multisamples > 1 && glRenderbufferStorageMultisampleEXT != NULL && glFramebufferTexture2DMultisampleEXT != NULL )
                {
                    // Create multisampled depth buffer.
                    GL( glGenRenderbuffers( 1, &frameBuffer->DepthBuffers[i] ) );
                    GL( glBindRenderbuffer( GL_RENDERBUFFER, frameBuffer->DepthBuffers[i] ) );
                    GL( glRenderbufferStorageMultisampleEXT( GL_RENDERBUFFER, multisamples, GL_DEPTH_COMPONENT24, width, height ) );
                    GL( glBindRenderbuffer( GL_RENDERBUFFER, 0 ) );

                    // Create the frame buffer.
                    // NOTE: glFramebufferTexture2DMultisampleEXT only works with GL_FRAMEBUFFER.
                    GL( glGenFramebuffers( 1, &frameBuffer->FrameBuffers[i] ) );
                    GL( glBindFramebuffer( GL_FRAMEBUFFER, frameBuffer->FrameBuffers[i] ) );
                    GL( glFramebufferTexture2DMultisampleEXT( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0, multisamples ) );
                    GL( glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, frameBuffer->DepthBuffers[i] ) );
                    GL( GLenum renderFramebufferStatus = glCheckFramebufferStatus( GL_FRAMEBUFFER ) );
                    GL( glBindFramebuffer( GL_FRAMEBUFFER, 0 ) );
                    if ( renderFramebufferStatus != GL_FRAMEBUFFER_COMPLETE )
                    {
                        ALOGE( "Incomplete frame buffer object: %s", GlFrameBufferStatusString( renderFramebufferStatus ) );
                        return false;
                    }
                }
                else
                {
                    // Create depth buffer.
                    GL( glGenRenderbuffers( 1, &frameBuffer->DepthBuffers[i] ) );
                    GL( glBindRenderbuffer( GL_RENDERBUFFER, frameBuffer->DepthBuffers[i] ) );
                    GL( glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height ) );
                    GL( glBindRenderbuffer( GL_RENDERBUFFER, 0 ) );

                    // Create the frame buffer.
                    GL( glGenFramebuffers( 1, &frameBuffer->FrameBuffers[i] ) );
                    GL( glBindFramebuffer( GL_DRAW_FRAMEBUFFER, frameBuffer->FrameBuffers[i] ) );
                    GL( glFramebufferRenderbuffer( GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, frameBuffer->DepthBuffers[i] ) );
                    GL( glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0 ) );
                    GL( GLenum renderFramebufferStatus = glCheckFramebufferStatus( GL_DRAW_FRAMEBUFFER ) );
                    GL( glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 ) );
                    if ( renderFramebufferStatus != GL_FRAMEBUFFER_COMPLETE )
                    {
                        ALOGE( "Incomplete frame buffer object: %s", GlFrameBufferStatusString( renderFramebufferStatus ) );
                        return false;
                    }
                }
            }
        }

        return true;
    }

    static void ovrFramebuffer_Destroy( ovrFramebuffer * frameBuffer )
    {
        GL( glDeleteFramebuffers( frameBuffer->TextureSwapChainLength, frameBuffer->FrameBuffers ) );
        if ( frameBuffer->UseMultiview )
        {
            GL( glDeleteTextures( frameBuffer->TextureSwapChainLength, frameBuffer->DepthBuffers ) );
        }
        else
        {
            GL( glDeleteRenderbuffers( frameBuffer->TextureSwapChainLength, frameBuffer->DepthBuffers ) );
        }
        vrapi_DestroyTextureSwapChain( frameBuffer->ColorTextureSwapChain );

        free( frameBuffer->DepthBuffers );
        free( frameBuffer->FrameBuffers );

        ovrFramebuffer_Clear( frameBuffer );
    }

    static void ovrFramebuffer_SetCurrent( ovrFramebuffer * frameBuffer )
    {
        GL( glBindFramebuffer( GL_DRAW_FRAMEBUFFER, frameBuffer->FrameBuffers[frameBuffer->TextureSwapChainIndex] ) );
    }

    static void ovrFramebuffer_SetNone()
    {
        GL( glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 ) );
    }

    static void ovrFramebuffer_Resolve( ovrFramebuffer * frameBuffer )
    {
        // Discard the depth buffer, so the tiler won't need to write it back out to memory.
        const GLenum depthAttachment[1] = { GL_DEPTH_ATTACHMENT };
        glInvalidateFramebuffer( GL_DRAW_FRAMEBUFFER, 1, depthAttachment );

        // Flush this frame worth of commands.
        glFlush();
    }

    static void ovrFramebuffer_Advance( ovrFramebuffer * frameBuffer )
    {
        // Advance to the next texture from the set.
        frameBuffer->TextureSwapChainIndex = ( frameBuffer->TextureSwapChainIndex + 1 ) % frameBuffer->TextureSwapChainLength;
    }



    static void ovrFence_Create( ovrFence * fence )
    {
#if defined( EGL_SYNC )
        fence->Display = 0;
        fence->Sync = EGL_NO_SYNC_KHR;
#else
        fence->Sync = 0;
#endif
    }

    static void ovrFence_Destroy( ovrFence * fence )
    {
#if defined( EGL_SYNC )
        if ( fence->Sync != EGL_NO_SYNC_KHR )
        {
            if ( eglDestroySyncKHR( fence->Display, fence->Sync ) ==  EGL_FALSE )
            {
                ALOGE( "eglDestroySyncKHR() : EGL_FALSE" );
                return;
            }
            fence->Display = 0;
            fence->Sync = EGL_NO_SYNC_KHR;
        }
#else
        if ( fence->Sync != 0 )
	{
		glDeleteSync( fence->Sync );
		fence->Sync = 0;
	}
#endif
    }

    static void ovrFence_Insert( ovrFence * fence )
    {
        ovrFence_Destroy( fence );

#if defined( EGL_SYNC )
        fence->Display = eglGetCurrentDisplay();
        fence->Sync = eglCreateSyncKHR( fence->Display, EGL_SYNC_FENCE_KHR, NULL );
        if ( fence->Sync == EGL_NO_SYNC_KHR )
        {
            ALOGE( "eglCreateSyncKHR() : EGL_NO_SYNC_KHR" );
            return;
        }
        // Force flushing the commands.
        // Note that some drivers will already flush when calling eglCreateSyncKHR.
        if ( eglClientWaitSyncKHR( fence->Display, fence->Sync, EGL_SYNC_FLUSH_COMMANDS_BIT_KHR, 0 ) == EGL_FALSE )
        {
            ALOGE( "eglClientWaitSyncKHR() : EGL_FALSE" );
            return;
        }
#else
        // Create and insert a new sync object.
	fence->Sync = glFenceSync( GL_SYNC_GPU_COMMANDS_COMPLETE, 0 );
	// Force flushing the commands.
	// Note that some drivers will already flush when calling glFenceSync.
	glClientWaitSync( fence->Sync, GL_SYNC_FLUSH_COMMANDS_BIT, 0 );
#endif
    }

    /**
     *
     * OvrSimulation methods
     *
     */

    static void ovrSimulation_Clear( ovrSimulation * simulation )
    {
        simulation->CurrentRotation.x = 0.0f;
        simulation->CurrentRotation.y = 0.0f;
        simulation->CurrentRotation.z = 0.0f;
    }

    static void ovrSimulation_Advance( ovrSimulation * simulation, double predictedDisplayTime )
    {
        // Update rotation.
        simulation->CurrentRotation.x = (float)( predictedDisplayTime );
        simulation->CurrentRotation.y = (float)( predictedDisplayTime );
        simulation->CurrentRotation.z = (float)( predictedDisplayTime );
    }


    /**
     *
     * OvrRenderer methods
     *
     */

    static void ovrRenderer_Clear( ovrRenderer * renderer )
    {
        for ( int eye = 0; eye < VRAPI_FRAME_LAYER_EYE_MAX; eye++ )
        {
            ovrFramebuffer_Clear( &renderer->FrameBuffer[eye] );
        }
        renderer->ProjectionMatrix = ovrMatrix4f_CreateIdentity();
        renderer->TexCoordsTanAnglesMatrix = ovrMatrix4f_CreateIdentity();
        renderer->NumBuffers = VRAPI_FRAME_LAYER_EYE_MAX;
    }

    static void ovrRenderer_Create( ovrRenderer * renderer, const ovrJava * java, const bool useMultiview )
    {
        renderer->NumBuffers = useMultiview ? 1 : VRAPI_FRAME_LAYER_EYE_MAX;

        // Create the frame buffers.
        for ( int eye = 0; eye < renderer->NumBuffers; eye++ )
        {
            ovrFramebuffer_Create( &renderer->FrameBuffer[eye], useMultiview,
                                   VRAPI_TEXTURE_FORMAT_8888,
                                   vrapi_GetSystemPropertyInt( java, VRAPI_SYS_PROP_SUGGESTED_EYE_TEXTURE_WIDTH ),
                                   vrapi_GetSystemPropertyInt( java, VRAPI_SYS_PROP_SUGGESTED_EYE_TEXTURE_HEIGHT ),
                                   NUM_MULTI_SAMPLES );

            renderer->Fence[eye] = (ovrFence *) malloc( renderer->FrameBuffer[eye].TextureSwapChainLength * sizeof( ovrFence ) );
            for ( int i = 0; i < renderer->FrameBuffer[eye].TextureSwapChainLength; i++ )
            {
                ovrFence_Create( &renderer->Fence[eye][i] );
            }
        }

        // Setup the projection matrix.
        renderer->ProjectionMatrix = ovrMatrix4f_CreateProjectionFov(
                vrapi_GetSystemPropertyFloat( java, VRAPI_SYS_PROP_SUGGESTED_EYE_FOV_DEGREES_X ),
                vrapi_GetSystemPropertyFloat( java, VRAPI_SYS_PROP_SUGGESTED_EYE_FOV_DEGREES_Y ),
                0.0f, 0.0f, 0.1f, 0.0f );
        renderer->TexCoordsTanAnglesMatrix = ovrMatrix4f_TanAngleMatrixFromProjection( &renderer->ProjectionMatrix );
    }

    static void ovrRenderer_Destroy( ovrRenderer * renderer )
    {
        for ( int eye = 0; eye < renderer->NumBuffers; eye++ )
        {
            for ( int i = 0; i < renderer->FrameBuffer[eye].TextureSwapChainLength; i++ )
            {
                ovrFence_Destroy( &renderer->Fence[eye][i] );
            }
            free( renderer->Fence[eye] );

            ovrFramebuffer_Destroy( &renderer->FrameBuffer[eye] );
        }
        renderer->ProjectionMatrix = ovrMatrix4f_CreateIdentity();
        renderer->TexCoordsTanAnglesMatrix = ovrMatrix4f_CreateIdentity();
    }

    void ovrRenderer_drawScene(Scene* scene)
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

    static ovrFrameParms ovrRenderer_RenderFrame( ovrRenderer * renderer, const ovrJava * java,
                                                  long long frameIndex, int minimumVsyncs, const ovrPerformanceParms * perfParms,
                                                  CameraGLOVR* camera,
                                                  Scene * scene, const ovrSimulation * simulation,
                                                  const ovrTracking * tracking, ovrMobile * ovr )
    {
        ovrFrameParms parms = vrapi_DefaultFrameParms( java, VRAPI_FRAME_INIT_DEFAULT, vrapi_GetTimeInSeconds(), NULL );
        parms.FrameIndex = frameIndex;
        parms.MinimumVsyncs = minimumVsyncs;
        parms.PerformanceParms = *perfParms;



        const ovrHeadModelParms headModelParms = vrapi_DefaultHeadModelParms();

#if REDUCED_LATENCY
        // Update orientation, not position.
	ovrTracking updatedTracking = vrapi_GetPredictedTracking( ovr, tracking->HeadPose.TimeInSeconds );
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
        projectionMatrixTransposed = ovrMatrix4f_Transpose( &renderer->ProjectionMatrix );



        for ( int eye = 0; eye < VRAPI_FRAME_LAYER_EYE_MAX; eye++ )
        {
            ovrFramebuffer * frameBuffer = &renderer->FrameBuffer[renderer->NumBuffers == 1 ? 0 : eye];
            parms.Layers[0].Textures[eye].ColorTextureSwapChain = frameBuffer->ColorTextureSwapChain;
            parms.Layers[0].Textures[eye].TextureSwapChainIndex = frameBuffer->TextureSwapChainIndex;
            parms.Layers[0].Textures[eye].TexCoordsFromTanAngles = renderer->TexCoordsTanAnglesMatrix;
            parms.Layers[0].Textures[eye].HeadPose = updatedTracking.HeadPose;
        }

        parms.Layers[0].Flags |= VRAPI_FRAME_LAYER_FLAG_CHROMATIC_ABERRATION_CORRECTION;

        unsigned long long completionFence[VRAPI_FRAME_LAYER_EYE_MAX] = { 0 };

        /* update camera projection matrix - it remains the same for each of the eye */
        CL_Mat44 sceneProjectionMat = CL_Make_Mat44(&projectionMatrixTransposed.M[0][0]);//&renderer->ProjectionMatrix.M[0][0]);
        camera->setProjectionMatrix(sceneProjectionMat);

        // Render the eye images.
        for ( int eye = 0; eye < renderer->NumBuffers; eye++ )
        {
           // NOTE: In the non-mv case, latency can be further reduced by updating the sensor prediction
            // for each eye (updates orientation, not position)

            /* Update the view matrix for each eye */
            CL_Mat44 sceneViewMat = CL_Make_Mat44(&eyeViewMatrixTransposed[eye].M[0][0]);
            camera->setViewMatrix(sceneViewMat);


            ovrFramebuffer * frameBuffer = &renderer->FrameBuffer[eye];
            ovrFramebuffer_SetCurrent( frameBuffer );


            GL( glEnable( GL_SCISSOR_TEST ) );
            GL( glEnable( GL_CULL_FACE ) );
            GL( glCullFace( GL_BACK ) );
            GL( glViewport( 0, 0, frameBuffer->Width, frameBuffer->Height ) );
            GL( glScissor( 0, 0, frameBuffer->Width, frameBuffer->Height ) );

            ovrRenderer_drawScene(scene);

//             Explicitly clear the border texels to black because OpenGL-ES does not support GL_CLAMP_TO_BORDER.
            {
                // Clear to fully opaque black.
                GL( glClearColor( 0.0f, 0.0f, 0.0f, 1.0f ) );
                // bottom
                GL( glScissor( 0, 0, frameBuffer->Width, 1 ) );
                GL( glClear( GL_COLOR_BUFFER_BIT ) );
                // top
                GL( glScissor( 0, frameBuffer->Height - 1, frameBuffer->Width, 1 ) );
                GL( glClear( GL_COLOR_BUFFER_BIT ) );
                // left
                GL( glScissor( 0, 0, 1, frameBuffer->Height ) );
                GL( glClear( GL_COLOR_BUFFER_BIT ) );
                // right
                GL( glScissor( frameBuffer->Width - 1, 0, 1, frameBuffer->Height ) );
                GL( glClear( GL_COLOR_BUFFER_BIT ) );
            }


            ovrFramebuffer_Resolve( frameBuffer );

            ovrFence * fence = &renderer->Fence[eye][frameBuffer->TextureSwapChainIndex];
            ovrFence_Insert( fence );
            completionFence[eye] = (size_t)fence->Sync;

            ovrFramebuffer_Advance( frameBuffer );
        }

        for ( int eye = 0; eye < VRAPI_FRAME_LAYER_EYE_MAX; eye++ )
        {
            parms.Layers[0].Textures[eye].CompletionFence =
                    completionFence[renderer->NumBuffers == 1 ? 0 : eye];
        }

        ovrFramebuffer_SetNone();

        return parms;
    }


    /**
     *
     * RendererGearVR class methods
     *
     */

    RendererGearVR::RendererGearVR(JNIEnv *env, jobject activityObject, ILoggerFactory* loggerFactory)
            : window(NULL),
              ovrM(NULL),
              frameIndex(1),
              minimumVSyncs(1),
              IRenderer()
    {
        this->logger = loggerFactory->createLogger("RendererGearVR");

        ovrEgl_Clear( &this->eglParams );

        ovrRenderer_Clear(&this->OVRRenderer);

        ovrSimulation_Clear(&this->OVRSimulation);

        env->GetJavaVM(&this->javaVM);
        this->java.Env = env;
        this->activityObject = env->NewGlobalRef(activityObject);
    }

    bool RendererGearVR::start() {
        java.Vm = this->javaVM;
        java.Vm->AttachCurrentThread(&java.Env, NULL);
        java.ActivityObject = this->activityObject;

        prctl(PR_SET_NAME, (long) "CL::Main", 0, 0, 0);


        this->logger->log(LOG_DEBUG, "vrapi_DefaultInitParms()");
        const ovrInitParms initParms = vrapi_DefaultInitParms(&java);
        this->logger->log(LOG_DEBUG, "vrapi_Initialize()");
        int32_t initResult = vrapi_Initialize(&initParms);

        if (initResult != VRAPI_INITIALIZE_SUCCESS) {
            char const *msg = initResult == VRAPI_INITIALIZE_PERMISSIONS_ERROR ?
                              "Thread priority security exception. Make sure the APK is signed." :
                              "VrApi initialization error.";
            logger->log(LOG_ERROR, "Thread priority security exception. Make sure the APK is signed."
                    "VrApi initialization error.");
        }
        logger->log(LOG_DEBUG, "eglParams.createEGLContext()");

        ovrEgl_CreateContext(&this->eglParams, nullptr);

        EglInitExtensions( &this->glExtensions );

        this->useMultiView &= (this->glExtensions.multi_view &&
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

        ovrRenderer_Create(&this->OVRRenderer, &java, this->useMultiView);

//        this->enterIntoVrMode();
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

        this->renderCamera = (CameraGLOVR*)cameraRelations[0];

        /**
         * TODO: vrapi initial frame submit
         */
        return true;
    }

    void RendererGearVR::update() { }


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

        ovrSimulation_Advance(&this->OVRSimulation, predictedDisplayTime);

        // Create the scene
        bool UseMultiview = this->useMultiView;

        // Render eye images and setup ovrFrameParms using ovrTracking.
        const ovrFrameParms frameParms = ovrRenderer_RenderFrame(&this->OVRRenderer, &this->java, this->frameIndex,
                                                                 this->minimumVSyncs, &this->perfParms, this->renderCamera,
                                                                  scene, &this->OVRSimulation,
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

    }

    void RendererGearVR::setWindow(ANativeWindow *window) {
        this->window = window;
    }

    ANativeWindow* RendererGearVR::getWindow(){
        return this->window;
    }

    ovrMobile** RendererGearVR::getOvr()
    {
        return &this->ovrM;
    }

    void RendererGearVR::stop(){

//        this->leaveVrMode();

        ovrRenderer_Destroy(&this->OVRRenderer);

        ovrEgl_DestroyContext( &this->eglParams );
        vrapi_Shutdown();

        this->java.Vm->DetachCurrentThread();


    }

    void RendererGearVR::enterIntoVrMode() {
        if (this->ovrM == NULL) {
            ovrModeParms parms = vrapi_DefaultModeParms(&this->java);
            // Must reset the FLAG_FULLSCREEN window flag when using a SurfaceView
            parms.Flags |= VRAPI_MODE_FLAG_RESET_WINDOW_FULLSCREEN;

            parms.Flags |= VRAPI_MODE_FLAG_NATIVE_WINDOW;
            parms.Display = (size_t) this->eglParams.Display;
            parms.WindowSurface = (size_t) this->window;
            parms.ShareContext = (size_t) this->eglParams.Context;

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


}