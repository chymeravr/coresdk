#ifndef GLIMPLEMENTATION_MODELGL_H
#define GLIMPLEMENTATION_MODELGL_H

#include <memory>
#include <coreEngine/renderObjects/Model.h>
#include <coreEngine/renderObjects/IRenderable.h>

namespace cl{
    class ModelGL : public Model, public IRenderable {
    public:
        ModelGL(const std::string &sceneId, IRelationStore *relationStore);
        IRenderable *getRenderable();

        void setModelMatrixId(std::unique_ptr<CL_GLuint> modelMatrixId);//To be set by respective shader
        bool initialize();
        void draw();
        void deinitialize();
    };
}

#endif //GLIMPLEMENTATION_MODELGL_H
