//
// Created by chimeralabs on 9/7/2016.
//

#include <hmdImplementations/RendererNoHMD.h>
#include <coreEngine/util/LoggerFactory.h>
#include <coreEngine/model/Shader.h>

namespace cl{
    RendererNoHMD::RendererNoHMD() {
        loggerPtr = &LoggerFactory::getInstance().getLoggerInstance("app::hmdImplementation::RendererNoHMD");
        loggerPtr->log(LOG_INFO, "Instance created");
        this->serviceGLPoolPtr = &IServiceGLPool::getInstance();
        sceneServicePtr = serviceGLPoolPtr->getISceneService();
        shaderServicePtr = serviceGLPoolPtr->getIShaderService();
        materialServicePtr = serviceGLPoolPtr->getIMaterialService();
        modelServicePtr = serviceGLPoolPtr->getIModelService();
    }

    RendererNoHMD::~RendererNoHMD() {
    }

    bool RendererNoHMD::start() {
        loggerPtr->log(LOG_INFO, "Starting renderer.");
        if((eglParams.display = eglGetDisplay(EGL_DEFAULT_DISPLAY)) == EGL_NO_DISPLAY) {
            loggerPtr->log(LOG_ERROR, "eglGetDisplay() returned error " + eglGetError());
            return false;
        }

        if (!eglInitialize(eglParams.display, &eglParams.majorVersion, &eglParams.minorVersion)) {
            loggerPtr->log(LOG_ERROR, "eglInitialize() returned error " + eglGetError());
            return false;
        }

        const EGLint attribs[] = {
                EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                EGL_BLUE_SIZE, 8,
                EGL_GREEN_SIZE, 8,
                EGL_RED_SIZE, 8,
                EGL_NONE
        };

        if (!eglChooseConfig(eglParams.display, attribs, &eglParams.config, 1, &eglParams.numConfigs)) {
            loggerPtr->log(LOG_ERROR, "eglChooseConfig() returned error " + eglGetError());
            //destroy();
            return false;
        }

        EGLint format;
        if (!eglGetConfigAttrib(eglParams.display, eglParams.config, EGL_NATIVE_VISUAL_ID, &format)) {
            loggerPtr->log(LOG_ERROR, "eglGetConfigAttrib() returned error " + eglGetError());
            //destroy();
            return false;
        }

        ANativeWindow_setBuffersGeometry(this->window, 0, 0, format);

        if (!(eglParams.surface = eglCreateWindowSurface(eglParams.display, eglParams.config, this->window, 0))) {
            loggerPtr->log(LOG_ERROR, "eglCreateWindowSurface() returned error " + eglGetError());
            //destroy();
            return false;
        }

        if (!(eglParams.context = eglCreateContext(eglParams.display, eglParams.config, 0, 0))) {
            loggerPtr->log(LOG_ERROR, "eglCreateContext() returned error " + eglGetError());
            //destroy();
            return false;
        }

        if (!eglMakeCurrent(eglParams.display, eglParams.surface, eglParams.surface, eglParams.context)) {
            loggerPtr->log(LOG_ERROR, "eglMakeCurrent() returned error " + eglGetError());
            //destroy();
            return false;
        }

        if (!eglQuerySurface(eglParams.display, eglParams.surface, EGL_WIDTH, &eglParams.width) ||
            !eglQuerySurface(eglParams.display, eglParams.surface, EGL_HEIGHT, &eglParams.height)) {
            loggerPtr->log(LOG_ERROR, "eglQuerySurface() returned error " + eglGetError());
            //destroy();
            return false;
        }
        loggerPtr->log(LOG_DEBUG, "EGL Params set");
        return true;
    }

    bool RendererNoHMD::initialize(Scene &scene) {
        loggerPtr->log(LOG_INFO, "Initializing");
/*
        Scene scene("House");
        scene.addModel(std::unique_ptr<Model>(new Model("Chair")));
        scene.addShader(std::unique_ptr<Shader>(new Shader("Default")));
        scene.addMaterial(std::unique_ptr<Material>(new Material("Wooden", scene.getShader("Default"))));
        scene.addModel(std::unique_ptr<Model>(new Model("Table")));
        Model &chair = scene.getModel("Chair");
        Model &table = scene.getModel("Table");
        Material &wooden = scene.getMaterial("Wooden");
        wooden.addModel(chair);
        table.setMaterial(wooden);
        chair.removeMaterial();
        wooden.addModel(chair);
        scene.removeShader(scene.getShader("Default"));
*/

        std::pair<bool, IObjectRenderer &> sceneRendererPair = sceneServicePtr->getSceneRenderer(scene);
        if(!sceneRendererPair.first){
            loggerPtr->log(LOG_ERROR, "sceneRenderer couldn't be retrieved from scene: " + scene.getTag());
        }
        sceneRendererPair.second.initialize(scene);
        std::vector<Object*> &shaders = sceneServicePtr->getObjectsByObjectType(scene,"shader");
        for(auto shaderIt=shaders.cbegin(); shaderIt!=shaders.cend(); shaderIt++){
            Shader &shader = (Shader&)(**shaderIt);
            std::pair<bool, IObjectRenderer&> shaderRendererPair = shaderServicePtr->getRenderer(shader);
            if(!shaderRendererPair.first){
                loggerPtr->log(LOG_ERROR, "shaderRenderer couldn't be retrieved from shader:" + shader.getTag());
                return false;
            }
            IObjectRenderer &shaderRenderer = shaderRendererPair.second;
            shaderRenderer.initialize(shader);

            std::vector<Material*> materials = shaderServicePtr->getMaterials(shader);
            for(auto materialIt=materials.cbegin(); materialIt!=materials.cend(); materialIt++){
                Material &material = **materialIt;
                std::pair<bool, IObjectRenderer&> materialRendererPair = materialServicePtr->getRenderer(material);
                if(!materialRendererPair.first){
                    loggerPtr->log(LOG_ERROR, "materialRenderer couldn't be retrieved from material: " + material.getTag());
                    return false;
                }
                IObjectRenderer &materialRenderer = materialRendererPair.second;
                materialRenderer.initialize(material);

                std::vector<Model*> models = materialServicePtr->getModelsOfMaterial(material);
                for(auto modelIt=models.cbegin(); modelIt!=models.cend(); modelIt++){
                    Model &model = **modelIt;
                    std::pair<bool, IObjectRenderer&> modelRendererPair = modelServicePtr->getRenderer(model);
                    if(!modelRendererPair.first){
                        loggerPtr->log(LOG_ERROR, "modelRenderer couldn't be retrieved from model: "+model.getTag());
                        return false;
                    }
                    IObjectRenderer &modelRenderer = modelRendererPair.second;
                    modelRenderer.initialize(model);
                }
            }
        }
        loggerPtr->log(LOG_DEBUG, "Initialization done");
    }

    void RendererNoHMD::update() {
    }

    void RendererNoHMD::draw(Scene &scene) {
        if (!eglSwapBuffers(eglParams.display, eglParams.surface)) {
            loggerPtr->log(LOG_ERROR, "eglSwapBuffers() returned error " + eglGetError());
        }
        loggerPtr->log(LOG_DEBUG, "Starting draw");
        if(&scene == NULL){
            loggerPtr->log(LOG_ERROR, "scene is null");
        }

        std::pair<bool, IObjectRenderer &> sceneRendererPair = sceneServicePtr->getSceneRenderer(scene);
        if(!sceneRendererPair.first){
            loggerPtr->log(LOG_ERROR, "sceneRenderer couldn't be retrieved from scene: " + scene.getTag());
        }
        sceneRendererPair.second.draw(scene);
        std::vector<Object*> &shaders = sceneServicePtr->getObjectsByObjectType(scene, "shader");
        if(&shaders == NULL){
            loggerPtr->log(LOG_ERROR, " shaders are null");
        }
        for(auto shaderIt=shaders.cbegin(); shaderIt!=shaders.cend(); shaderIt++){
            Shader &shader = (Shader&)(**shaderIt);
            std::pair<bool, IObjectRenderer&> shaderRendererPair = shaderServicePtr->getRenderer(shader);
            if(!shaderRendererPair.first){
                loggerPtr->log(LOG_ERROR, "shaderRenderer couldn't be retrieved from shader:" + shader.getTag());
                return;
            }
            IObjectRenderer &shaderRenderer = shaderRendererPair.second;
            shaderRenderer.draw(shader);
            std::vector<Material*> materials = shaderServicePtr->getMaterials(shader);
            for(auto materialIt=materials.cbegin(); materialIt!=materials.cend(); materialIt++){
                Material &material = **materialIt;
                std::pair<bool, IObjectRenderer&> materialRendererPair = materialServicePtr->getRenderer(material);
                if(!materialRendererPair.first){
                    loggerPtr->log(LOG_ERROR, "materialRenderer couldn't be retrieved from material: " + material.getTag());
                    return;
                }
                IObjectRenderer &materialRenderer = materialRendererPair.second;
                materialRenderer.draw(material);

                std::vector<Model *> models = materialServicePtr->getModelsOfMaterial(material);
                for(auto modelIt=models.cbegin(); modelIt!=models.cend(); modelIt++){
                    Model &model = **modelIt;
                    std::pair<bool, IObjectRenderer&> modelRendererPair = modelServicePtr->getRenderer(model);
                    if(!modelRendererPair.first){
                        loggerPtr->log(LOG_ERROR, "modelRenderer couldn't be retrieved from model: "+model.getTag());
                        return;
                    }
                    IObjectRenderer &modelRenderer = modelRendererPair.second;
                    modelRenderer.draw(model);
                }
            }
        }
        loggerPtr->log(LOG_DEBUG, "Draw done.");
    }

    void RendererNoHMD::deinitialize(Scene &scene) {

        std::pair<bool, IObjectRenderer &> sceneRendererPair = sceneServicePtr->getSceneRenderer(scene);
        if(!sceneRendererPair.first){
            loggerPtr->log(LOG_ERROR, "sceneRenderer couldn't be retrieved from scene: " + scene.getTag());
        }
        sceneRendererPair.second.deinitialize(scene);
        std::vector<Object*> &shaders = sceneServicePtr->getObjectsByObjectType(scene, "shader");
        for(auto shaderIt=shaders.cbegin(); shaderIt!=shaders.cend(); shaderIt++){
            Shader &shader = (Shader&)(**shaderIt);
            std::pair<bool, IObjectRenderer&> shaderRendererPair = shaderServicePtr->getRenderer(shader);
            if(!shaderRendererPair.first){
                loggerPtr->log(LOG_ERROR, "shaderRenderer couldn't be retrieved from shader:" + shader.getTag());
                return;
            }
            IObjectRenderer &shaderRenderer = shaderRendererPair.second;
            shaderRenderer.deinitialize(shader);

            std::vector<Material*> materials = shaderServicePtr->getMaterials(shader);
            for(auto materialIt=materials.cbegin(); materialIt!=materials.cend(); materialIt++){
                Material &material = **materialIt;
                std::pair<bool, IObjectRenderer&> materialRendererPair = materialServicePtr->getRenderer(material);
                if(!materialRendererPair.first){
                    loggerPtr->log(LOG_ERROR, "materialRenderer couldn't be retrieved from material: " + material.getTag());
                    return;
                }
                IObjectRenderer &materialRenderer = materialRendererPair.second;
                materialRenderer.deinitialize(material);

                std::vector<Model*> models = materialServicePtr->getModelsOfMaterial(material);
                for(auto modelIt=models.cbegin(); modelIt!=models.cend(); modelIt++){
                    Model &model = **modelIt;
                    std::pair<bool, IObjectRenderer&> modelRendererPair = modelServicePtr->getRenderer(model);
                    if(!modelRendererPair.first){
                        loggerPtr->log(LOG_ERROR, "modelRenderer couldn't be retrieved from model: "+model.getTag());
                        return;
                    }
                    IObjectRenderer &modelRenderer = modelRendererPair.second;
                    modelRenderer.deinitialize(model);
                }
            }
        }
    }

    void RendererNoHMD::destroy() {
        eglMakeCurrent(eglParams.display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        eglDestroyContext(eglParams.display, eglParams.context);
        eglDestroySurface(eglParams.display, eglParams.surface);
        eglTerminate(eglParams.display);

        eglParams.display = EGL_NO_DISPLAY;
        eglParams.surface = EGL_NO_SURFACE;
        eglParams.context = EGL_NO_CONTEXT;
    }

    void RendererNoHMD::setWindow(ANativeWindow *window) {
        this->window = window;
        loggerPtr->log(LOG_INFO, "Windows Set");
    }
}