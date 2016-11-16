//
// Created by chimeralabs on 10/24/2016.
//

#ifndef ANDROIDSDK_MODELGLRENDERER_H
#define ANDROIDSDK_MODELGLRENDERER_H

#include <coreEngine/model/IObjectRenderer.h>
#include <coreEngine/service/IComponentService.h>
#include <conf/GLTypes.h>
#include <glImplementation/model/ModelGL.h>
#include <coreEngine/service/IMaterialService.h>
#include <coreEngine/service/IModelService.h>
#include <glImplementation/service/IShaderGLService.h>

namespace cl{
    class ModelGLRenderer : public IObjectRenderer{
    private:
        Logger *loggerPtr;
        std::unique_ptr<IComponentService> componentServicePtr;
        std::unique_ptr<IMaterialService> materialServicePtr;
        std::unique_ptr<IShaderGLService> shaderGLServicePtr;
        std::unique_ptr<IModelService> modelServicePtr;
    public:
        ModelGLRenderer();
        bool initialize(Object &object);
        bool draw(Object &object);
        bool deinitialize(Object &object);

    private:
        void createVertexBuffer(ModelGL &modelGL);
        void useVertexBuffer(ModelGL &modelGL);
        void destroyVertexBuffer(ModelGL &modelGL);

        void createUVBuffer(ModelGL &modelGL);
        void useUVBuffer(ModelGL &modelGL);
        void destroyUVBuffer(ModelGL &modelGL);

        void createColorBuffer(ModelGL &modelGL);
        void useColorBuffer(ModelGL &modelGL);
        void destroyColorBuffer(ModelGL &modelGL);

        void createNormalBuffer(ModelGL &modelGL);
        void useNormalBuffer(ModelGL &modelGL);
        void destroyNormalBuffer(ModelGL &modelGL);

        void createIndexBuffer(ModelGL &modelGL);
        void useIndexBuffer(ModelGL &modelGL);
        void destroyIndexBuffer(ModelGL &modelGL);
    };
}

#endif //ANDROIDSDK_MODELGLRENDERER_H
