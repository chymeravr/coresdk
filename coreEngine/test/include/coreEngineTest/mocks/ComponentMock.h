#ifndef ANDROIDSDK_COMPONENTMOCK_H
#define ANDROIDSDK_COMPONENTMOCK_H


#include <gmock/gmock.h>
#include <coreEngine/model/IComponent.h>

namespace cl{
    class ComponentMock : public IComponent{
    private:
        Object *objectPtr;
    public:
        std::string getComponentType(){
            return "component";
        }
        MOCK_METHOD0(initialize, void());
        MOCK_METHOD0(draw, void());
        MOCK_METHOD0(deinitialize, void());
        void setObject(Object *objectPtr){
            this->objectPtr = objectPtr;
        }
        Object *getObject(){
            return this->objectPtr;
        }
    };
}

#endif //ANDROIDSDK_COMPONENTMOCK_H