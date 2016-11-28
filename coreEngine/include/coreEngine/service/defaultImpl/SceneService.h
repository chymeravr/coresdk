//
// Created by chimeralabs on 10/21/2016.
//

#ifndef ANDROIDSDK_SCENESERVICE_H
#define ANDROIDSDK_SCENESERVICE_H

#include <coreEngine/service/ISceneService.h>
#include <coreEngine/util/ILogger.h>
#include <coreEngine/util/ILoggerFactory.h>

namespace cl{
    class SceneService : public ISceneService{
    private:
		std::unique_ptr<ILogger> loggerPtr;
    public:
		SceneService(ILoggerFactory *loggerFactoryPtr);
		/**
		* Creates an Object in the Scene and stores it.
		* @args scenePtr: Pointer to the Scene object that stores the scene
		* @args factoryPtr: Pointer to the Factory object that implements IObjectFactory interface and determines how the object is created
		* @args tag: A unique identifier tag for the scene. Should be checked with isTagAvailable before creation.
		* @return Returns object pointer if creation was successful else returns nullptr.
		*/
		Object* createObject(Scene *scenePtr, IObjectFactory *factoryPtr, const std::string &tag);
		/**
		* Removes the object from scene.
		* @args scenePtr: Pointer to the Scene object that stores the scene
		* @args factoryPtr: Pointer to the Factory object that implements IObjectFactory interface and determines how the object is created
		* @args tag: A unique identifier tag for the scene. Should be checked with isTagAvailable before creation.
		*/
		void removeObject(Scene *scenePtr, IObjectFactory *factoryPtr, Object *objectPtr);

		/**
		* Returns true if the tag is available else false
		*/
		bool isTagAvailable(Scene *scenePtr, const std::string &tag);

		/**
		* Retrieves the object with the specified tag. Returns nullptr if it can't be retrieved.
		*/
		Object *getObjectByTag(Scene *scenePtr, const std::string &tag);

		/**
		* Retrieves the list of object with the specified type.
		*/
		std::vector<Object*> getObjectsByObjectType(Scene *scenePtr, const std::string &objectType);
    };
}

#endif //ANDROIDSDK_SCENESERVICE_H
