#ifndef COREENGINE_SHADER_H
#define COREENGINE_SHADER_H

#include <string>
#include <coreEngine/renderObjects/Relation.h>
#include <coreEngine/renderObjects/IScenable.h>
#include <coreEngine/renderObjects/IRenderable.h>
#include <coreEngine/renderObjects/IRelationStore.h>

namespace cl{
    class Shader : public Relation, public IScenable{
    public:
        Shader(const std::string &sceneId, IRelationStore *relationStore);
        virtual IRenderable *getRenderable() = 0;
        std::string getType();
        std::string getSceneId();
    };
}

#endif //COREENGINE_SHADER_H