//
// Created by chimeralabs on 10/24/2016.
//

#ifndef ANDROIDSDK_UNIQUEPOINTERHELPER_H
#define ANDROIDSDK_UNIQUEPOINTERHELPER_H

#include <memory>

namespace cl{
	/**
	 * Usage: unique_ptr<Scene>scenePtr = static_cast_ptr<Scene>(sceneFactoryPtr->createObject(sceneFactoryParam));
	 * Here createObject returns unique_ptr<Object>
	 */
	template<typename D, typename B>
	std::unique_ptr<D> static_cast_ptr(std::unique_ptr<B>& base)
	{
		return std::unique_ptr<D>(static_cast<D*>(base.release()));
	}

	/**
	 * Usage: unique_ptr<Scene>scenePtr = static_cast_ptr<Scene>(sceneFactoryPtr->createObject(sceneFactoryParam));
	 * Here createObject returns unique_ptr<Object>
	 */
	template<typename D, typename B>
	std::unique_ptr<D> static_cast_ptr(std::unique_ptr<B>&& base)
	{
		return std::unique_ptr<D>(static_cast<D*>(base.release()));
	}
}

#endif //ANDROIDSDK_UNIQUEPOINTERHELPER_H
