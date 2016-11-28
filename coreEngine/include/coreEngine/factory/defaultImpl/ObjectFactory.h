#ifndef ANDROIDSDK_OBJECTFACTORY_H
#define ANDROIDSDK_OBJECTFACTORY_H

#include <coreEngine/factory/IObjectFactory.h>

namespace cl{
	class ObjectFactory : public IObjectFactory{
	public:
		std::unique_ptr<Object> createObject(std::string tag){
			return std::unique_ptr<Object>(new Object("Object", tag));
		}
		void destroyObject(Object *objectPtr){

		}
	};
}

#endif //ANDROIDSDK_OBJECTFACTORY_H
