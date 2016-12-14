#ifndef COREENGINE_MATERIAL_H
#define COREENGINE_MATERIAL_H

#include <coreEngine/renderObjects/Relation.h>
#include <coreEngine/renderObjects/IScenable.h>
#include <coreEngine/renderObjects/Shader.h>
#include <coreEngine/renderObjects/IRenderable.h>

namespace cl{
    class Material : public Relation, public IScenable{
    public:
        Material(const std::string &sceneId, IRelationStore *relationStore, Shader *shader);
        virtual IRenderable *getRenderable() = 0;
        std::string getType();
        std::string getSceneId();
    };
}

#endif //COREENGINE_MATERIAL_H