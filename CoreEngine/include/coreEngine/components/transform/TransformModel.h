#ifndef COREENGINE_TRANSFORMMODEL_H
#define COREENGINE_TRANSFORMMODEL_H

#include <coreEngine/renderObjects/Model.h>
#include <coreEngine/components/transform/Transform.h>
#include <coreEngine/renderObjects/IRenderable.h>

namespace cl{
    class TransformModel : public Transform, public IRenderable{
    public:
        TransformModel(Model *model);
        virtual IRenderable *getRenderable();
        virtual bool initialize();
        virtual void draw();
        virtual void deinitialize();
    };
}

#endif //COREENGINE_TRANSFORMMODEL_H