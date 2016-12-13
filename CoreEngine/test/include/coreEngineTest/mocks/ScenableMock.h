#ifndef COREENGINETEST_SCENABLEMOCK_H
#define COREENGINETEST_SCENABLEMOCK_H

#include <gmock/gmock.h>
#include <coreEngine/renderObjects/IScenable.h>

namespace cl{
    class ScenableMock : public IScenable{
    public:
        ScenableMock(std::string sceneId){
            this->sceneId = sceneId;
        }
        std::string getSceneId(){
            return this->sceneId;
        }
    private:
        std::string sceneId;
    };
}

#endif //COREENGINETEST_SCENABLEMOCK_H