//
// Created by chimeralabs on 10/26/2016.
//

#ifndef ANDROIDSDK_OBJECTSERVICE_H
#define ANDROIDSDK_OBJECTSERVICE_H

#include <coreEngine/service/IObjectService.h>
#include <coreEngine/util/ILoggerFactory.h>


namespace cl{
    class ObjectService : public IObjectService{
    private:
        std::unique_ptr<ILogger> loggerPtr;
    public:
		ObjectService(ILoggerFactory *loggerFactoryPtr);
		void linkObject(Object *objectPtr1, Object *objectPtr2);
		void unlinkObject(Object *objectPtr1, Object *objectPtr2);
		Object* getLinkedObjectByTag(Object *objectPtr, const std::string &tag);
		std::vector<Object *> getLinkedObjectsByObjectType(Object *objectPtr, const std::string &type);
    };
}

#endif //ANDROIDSDK_OBJECTSERVICE_H
