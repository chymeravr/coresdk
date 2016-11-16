//
// Created by chimeralabs on 8/8/2016.
//

#ifndef ANDROIDSDK_IMAGE360_H
#define ANDROIDSDK_IMAGE360_H

/**
 * Todo: Separate Objects in coreEngine and glImplementation. Don't derive them into glImplementation. There will be a GLObjectStore equivalent to ObjectStore with index as tag. Application will just make Scene object in core-Engine domain. And pass scene object to Renderer. Renderer will then decide which implementation or glImplementation to use or not. And accordingly create gl library specific objects. Addition and deletion of objects will be handled in runtime by checking if a tag is available or not.
 *
 */

#include <GLES2/gl2.h>
#include <pthread.h>
#include <memory>
#include <queue>
#include <coreEngine/util/Logger.h>
#include <coreEngine/model/IRenderer.h>
#include <coreEngine/factory/IFactoryPool.h>
#include <coreEngine/service/IServicePool.h>
#include <coreEngine/util/EventQueue.h>

namespace cl {
    enum MSG {
        MSG_RENDER_LOOP_EXIT,
        MSG_WINDOW_SET
    };

    class Image360 {

    private: //params
        Logger *loggerPtr;
        std::unique_ptr<IRenderer> rendererPtr;
        std::unique_ptr<IFactoryPool> factoryPoolPtr;
        IServicePool *servicePoolPtr;
        std::unique_ptr<ISceneService> sceneServicePtr;
        std::unique_ptr<IComponentService> componentServicePtr;
        std::unique_ptr<Scene> scenePtr;
        std::unique_ptr<ComponentStore> componentStorePtr;
        std::unique_ptr<IObjectRenderer> modelRendererPtr;
        std::unique_ptr<IObjectRenderer> shaderRendererPtr;
        std::unique_ptr<IObjectRenderer> materialUniformColorRenderer;
        std::unique_ptr<IObjectRenderer> sceneRendererPtr;
        std::string sceneTag;
        bool isRenderingOn = false;
        bool isLoopActive = true;
        pthread_t renderThreadId;
        pthread_mutex_t msgMutex;
        std::queue<MSG> msgQueue;
        EventQueue* eventQueue;

    public:
    private: //methods
        void renderLoop();
        static void *renderThreadStartCallback(void *context);
        void eventHandler(Event event);

    public: //methods
        Image360(std::unique_ptr<IRenderer> rendererPtr, std::unique_ptr<IFactoryPool> factoryPoolPtr);

        ~Image360();

        bool initialize();

        void draw();

        bool destroy();

        void startRenderThread();

        void stopRenderThread();

        IRenderer &getRenderer();
    };
}
#endif //ANDROIDSDK_IMAGE360_H
