#ifndef COREENGIN_SCENE_H
#define COREENGIN_SCENE_H

#include <memory>
#include <coreEngine/renderObjects/IRenderable.h>
#include <coreEngine/renderObjects/IRelationable.h>
#include <coreEngine/renderObjects/IScenable.h>

namespace cl{
class Scene : public IRelationable{
    public:
        virtual IRenderable *getRenderable() = 0;
        virtual void addToScene(std::unique_ptr<IScenable> scenable);
        virtual IScenable *getFromScene(std::string sceneTag);
        virtual void removeFromScene(IScenable *scenable);
        virtual bool exists(std::string sceneTag);
    };
/*
    class MaterialUniformColorGL : public MaterialUniformColor{
    public:
        IRenderable *getRenderable(){}
    private:
        MaterialUniformColor *material;
    };

    class SceneGL : public IRenderable{
        Scene scene;
    };

    */
}

#endif //COREENGIN_SCENE_H