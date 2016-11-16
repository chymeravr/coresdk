//
// Created by chimeralabs on 10/26/2016.
//

#ifndef ANDROIDSDK_OBJECTSERVICE_H
#define ANDROIDSDK_OBJECTSERVICE_H

#include <coreEngine/service/IObjectService.h>
#include <coreEngine/util/Logger.h>
#include <coreEngine/util/LoggerFactory.h>


namespace cl{
    class ObjectService : public IObjectService{
    private:
        Logger *loggerPtr;
    public:
        ObjectService();
        void linkObject(Object &object1, Object &object2);
        void unlinkObject(Object &object1, Object &object2);
        std::pair<bool, Object&> getLinkedObjectByTag(Object &object, std::string tag);
        std::pair<bool, std::vector<Object *>& > getLinkedObjectsByObjectType(Object &object, std::string type);
    };
}

#endif //ANDROIDSDK_OBJECTSERVICE_H
