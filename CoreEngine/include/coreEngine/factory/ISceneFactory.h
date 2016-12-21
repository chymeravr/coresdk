#ifndef COREENGINE_ISCENEFACTORY_H
#define COREENGINE_ISCENEFACTORY_H

#include <memory>
#include <coreEngine/renderObjects/Scene.h>

namespace cl{
    class ISceneFactory{
    public:
        virtual std::unique_ptr<Scene> create(std::string id) = 0;
    };
}

#endif //COREENGINE_ISCENEFACTORY_H