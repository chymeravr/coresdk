//
// Created by chimeralabs on 8/8/2016.
//

#include <image360/Image360.h>
#include <coreEngine/components/TransformFactory.h>
#include <coreEngine/util/LoggerFactory.h>
#include <coreEngine/factory/defaultImpl/SceneFactoryParam.h>
#include <coreEngine/util/UniquePointerHelper.h>
#include <coreEngine/factory/defaultImpl/CameraFactoryParam.h>
#include <coreEngine/factory/defaultImpl/ModelFactoryParam.h>
#include <coreEngine/factory/defaultImpl/ShaderUniformColorFactoryParam.h>
#include <coreEngine/factory/defaultImpl/MaterialUniformColorFactoryParam.h>

namespace cl {

    Image360::Image360(std::unique_ptr<IRenderer> rendererPtr, std::unique_ptr<IFactoryPool> factoryPoolPtr_) {
        loggerPtr = &LoggerFactory::getInstance().getLoggerInstance("app::applications::Image360:");

        loggerPtr->log(LOG_INFO, "Instance created");
        this->rendererPtr = std::move(rendererPtr);
        this->factoryPoolPtr = std::move(factoryPoolPtr_);
        this->servicePoolPtr = &IServicePool::getInstance();
        std::unique_ptr<IObjectFactory> sceneFactoryPtr = this->factoryPoolPtr->getSceneFactoryImpl();
        if(!sceneFactoryPtr){
            loggerPtr->log(LOG_ERROR, "factoryPool.getScenefactoryImpl() failed");
        }
        sceneRendererPtr = this->factoryPoolPtr->getSceneRendererImpl();
        SceneFactoryParam sceneFactoryParam(*sceneRendererPtr, "scene");
        if(!(sceneServicePtr = servicePoolPtr->getISceneService())){
            loggerPtr->log(LOG_ERROR, "Failed to create ISceneService");
        }

        if(!(scenePtr = static_cast_ptr<Scene>(sceneFactoryPtr->createObject(sceneFactoryParam)))){
            loggerPtr->log(LOG_ERROR, "Failed to create Scene with tag="+sceneFactoryParam.getTag());
        }
        componentServicePtr = servicePoolPtr->getIComponentService();
        componentStorePtr = std::unique_ptr<ComponentStore>(new ComponentStore);
        eventQueue = EventQueue::getInstance();
    }

    Image360::~Image360() {
        loggerPtr->log(LOG_INFO, "Instance destroyed");
    }

    void Image360::stopRenderThread() {
        loggerPtr->log(LOG_INFO, "Stopping render thread");
        pthread_mutex_lock(&msgMutex);
        msgQueue.push(MSG_RENDER_LOOP_EXIT);
        pthread_mutex_unlock(&msgMutex);
        pthread_join(renderThreadId, 0);
        loggerPtr->log(LOG_INFO, "Render thread stopped");
    }

    void Image360::startRenderThread() {
        loggerPtr->log(LOG_INFO, "Creating Render Thread");
        pthread_create(&renderThreadId, 0, renderThreadStartCallback, this);
    }

    void *Image360::renderThreadStartCallback(void *ctx) {
        Image360 *context = (Image360 *) ctx;
        context->renderLoop();
        pthread_exit(0);
        return 0;
    }

    void Image360::renderLoop() {
        loggerPtr->log(LOG_INFO, "Starting render loop");
        /*while(1){
            if(renderer->isReadyToRender()) {
                pthread_mutex_lock(&msgMutex);
                msgQueue.push(MSG_WINDOW_SET);
                pthread_mutex_unlock(&msgMutex);
                break;
            }
        }
        isDisplay = true;
        initialize();*/
        while (this->isLoopActive) {
            while (!eventQueue->empty()) {
                this->eventHandler(eventQueue->pop());

//                pthread_mutex_lock(&msgMutex);
//                MSG msg = msgQueue.front();
//                msgQueue.pop();
//                switch (msg) {
//                    /*case MSG_WINDOW_SET:
//                        logger->log(LOG_INFO, "Image360HMD: MSG_WINDOW_SET");
//                        initialize();
//                        isDisplay = true;
//                        break;*/
//                    case MSG_RENDER_LOOP_EXIT:
//                        logger->log(LOG_INFO, "Image360HMD: MSG_RENDER_LOOP_EXIT");
//                        isRenderingOn = false;
//                        break;
//                    default:
//                        break;
//                }
//                pthread_mutex_unlock(&msgMutex);
            }
            if(this->isRenderingOn){
                draw();
            }
        }

        loggerPtr->log(LOG_INFO, "Stopping render loop");
    }

    void Image360::eventHandler(Event event) {
        switch (event.getEventType()){
            case Event::START:
            case Event::PLAY:
                loggerPtr->log(LOG_DEBUG, "Event play");
                this->initialize();
                this->isRenderingOn = true;
                break;
            case Event::STOP:
            case Event::PAUSE:
                loggerPtr->log(LOG_DEBUG, "Event pause");
                this->isRenderingOn = false;
                break;
            case Event::ON_KEY_DOWN:
                break;
            case Event::ON_TOUCH:
                break;
            default:
                break;
        }
    }

    bool Image360::initialize() {
        loggerPtr->log(LOG_INFO, "Initializing");
        rendererPtr->start();
        //scenePtr = std::unique_ptr<Scene>(new Scene(sceneTag));

        glClearColor(0.0f, 0.0f, 0.5f, 1.0f);

        SCENE_ERROR_CODE code;
        ISceneService &sceneService = *sceneServicePtr;

        auto cameraFactoryPtr = factoryPoolPtr->getCameraFactoryImpl();
        if(!cameraFactoryPtr){
            loggerPtr->log(LOG_ERROR, "Failed to get camera Factory");
        }
        CameraFactoryParam cameraFactoryParam("mainCamera");
        code = sceneService.createObject(*scenePtr, *cameraFactoryPtr.get(), cameraFactoryParam);
        if(code != SCENE_NO_ERROR){
            loggerPtr->log(LOG_ERROR, "Failed to get object. Tag = " + cameraFactoryParam.getTag());
        }
        std::pair<bool, Object&> cameraPair = sceneService.getObjectByTag(*scenePtr, "mainCamera");
        if(!cameraPair.first){
            loggerPtr->log(LOG_ERROR, "couldn't retrieve object with tag=mainCamera");
        }
        Camera &camera = (Camera &)cameraPair.second;

        auto modelFactoryPtr = factoryPoolPtr->getModelFactoryImpl();
        if(!modelFactoryPtr){
            loggerPtr->log(LOG_ERROR, "Failed to get model Factory");
        }
        modelRendererPtr = factoryPoolPtr->getModelRendererImpl();
        if(!modelRendererPtr){
            loggerPtr->log(LOG_ERROR, "Failed to get Model Renderer");
        }
        ModelFactoryParam modelFactoryParam ("chair", *modelRendererPtr.get());
        code = sceneService.createObject(*scenePtr, *modelFactoryPtr.get(), modelFactoryParam);
        if(code != SCENE_NO_ERROR){
            loggerPtr->log(LOG_ERROR, "Failed to get object. Tag = " + modelFactoryParam.getTag());
        }
        std::pair<bool, Object&> chairPair = sceneService.getObjectByTag(*scenePtr, "chair");
        if(!chairPair.first){
            loggerPtr->log(LOG_ERROR, "couldn't retrieve object with tag=chair");
        }
        Model &chair = (Model &)chairPair.second;
        TransformFactory transformFactory;
        componentServicePtr->createComponent(*componentStorePtr, transformFactory, chair);
        std::pair<bool, IComponent &> transformPair = componentServicePtr->getComponent(chair, "transform");
        if(!transformPair.first){
            loggerPtr->log(LOG_ERROR, "coudln't retrieve component with type transform from object:"+chair.getTag());
        }
        ITransform &transform = (ITransform&)transformPair.second;
        if(&transform == NULL){
            loggerPtr->log(LOG_ERROR, "Returned component is null");
        }
        transform.setScene(*scenePtr);
        transform.setPosition(CL_Vec3(0.0, 0.0, 5.0));
        transform.setRoation(CL_Vec3(0.0, 0.0, 0.0));
        transform.setScale(CL_Vec3(1.0, 1.0, 1.0));

        auto shaderFactoryPtr = factoryPoolPtr->getShaderUniformColorFactoryImpl();
        if(!shaderFactoryPtr){
            loggerPtr->log(LOG_ERROR, "Failed to get Shader Factory");
        }
        shaderRendererPtr = factoryPoolPtr->getShaderUniformColorRendererImpl();
        if(!shaderRendererPtr){
            loggerPtr->log(LOG_ERROR, "Failed to get Shader Renderer");
        }
        ShaderUniformColorFactoryParam shaderFactoryParam ("uniformColorShader", *shaderRendererPtr.get());
        code = sceneService.createObject(*scenePtr, *shaderFactoryPtr.get(), shaderFactoryParam);
        if(code != SCENE_NO_ERROR){
            loggerPtr->log(LOG_ERROR, "Failed to get object. Tag = " + shaderFactoryParam.getTag());
        }
        std::pair<bool, Object&> shaderPair = sceneService.getObjectByTag(*scenePtr, "uniformColorShader");
        if(!shaderPair.first){
            loggerPtr->log(LOG_ERROR, "Couldn't retrieve object with tag uniformColorShader");
        }
        Shader &shader = (Shader&)shaderPair.second;

        auto materialUniformColorFactory = factoryPoolPtr->getMaterialUniformColorFactoryImpl();
        if(!materialUniformColorFactory){
            loggerPtr->log(LOG_ERROR, "Failed to get material uniform color factory.");
        }
        materialUniformColorRenderer = factoryPoolPtr->getMaterialUniformColorRendererImpl();
        if(!materialUniformColorRenderer){
            loggerPtr->log(LOG_ERROR, "Failed to get material uniform color renderer.");
        }
        MaterialUniformColorFactoryParam materialUniformColorFactoryParam("wooden", shader, *materialUniformColorRenderer.get());
        code = sceneService.createObject(*scenePtr, *materialUniformColorFactory.get(), materialUniformColorFactoryParam);
        std::pair<bool, Object&>  woodenMaterialPair = sceneService.getObjectByTag(*scenePtr, "wooden");
        if(!woodenMaterialPair.first){
            loggerPtr->log(LOG_ERROR, "Couldn't retrieve object with tag wooden");
        }
        Material &woodenMaterial = (Material &)woodenMaterialPair.second;

        auto materialServicePtr = servicePoolPtr->getIMaterialService(); //select implementation
        if(!materialServicePtr){
            loggerPtr->log(LOG_ERROR, "Failed to get material service");
        }
        materialServicePtr->addMaterialToModel(woodenMaterial, chair);

        rendererPtr->initialize(*scenePtr);

        /*
        glDisable(GL_DITHER);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
        glClearColor(0, 0, 0, 0);
        glEnable(GL_CULL_FACE);
        glShadeModel(GL_SMOOTH);
        glEnable(GL_DEPTH_TEST);

        glViewport(0, 0, eglParams.width, eglParams.height);

        GLfloat ratio = (GLfloat) eglParams.width / eglParams.height;
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glFrustumf(-ratio, ratio, -1, 1, 1, 10);*/
    }

    void Image360::draw(){
        glClear(GL_COLOR_BUFFER_BIT);
        rendererPtr->draw(*scenePtr);
    }

    bool Image360::destroy() {

        rendererPtr->destroy();
    }

    IRenderer& Image360::getRenderer() {
        return *rendererPtr;
    }

}