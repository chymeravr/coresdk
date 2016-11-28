#ifndef ANDROIDSDK_IMODELRENDERER_H
#define ANDROIDSDK_IMODELRENDERER_H

namespace cl{
    class Model;
    class IModelRenderer{
        virtual bool initialize(Model *model) = 0;
        virtual void draw(Model *model) = 0;
        virtual void deinitialize(Model *model) = 0;
    };
}

#endif //ANDROIDSDK_IMODELRENDERER_H