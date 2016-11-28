//
// Created by chimeralabs on 10/15/2016.
//

#ifndef ANDROIDSDK_ISCENESERVICE_H
#define ANDROIDSDK_ISCENESERVICE_H

#include <string>
#include <vector>

#include <coreEngine/model/Object.h>
#include <coreEngine/model/Scene.h>
#include <coreEngine/factory/IObjectFactory.h>

namespace cl{
    class ISceneService{
    public:
		/**
		* Creates an Object in the Scene and stores it.
		* @args scenePtr: Pointer to the Scene object that stores the scene
		* @args factoryPtr: Pointer to the Factory object that implements IObjectFactory interface and determines how the object is created
		* @args tag: A unique identifier tag for the scene. Should be checked with isTagAvailable before creation.
		* * @return Returns object pointer if creation was successful else returns nullptr.
		*/
        virtual Object* createObject(Scene *scenePtr, IObjectFactory *factoryPtr, const std::string &tag) = 0;
		/**
		* Removes the object from scene.
		* @args scenePtr: Pointer to the Scene object that stores the scene
		* @args factoryPtr: Pointer to the Factory object that implements IObjectFactory interface and determines how the object is created
		* @args tag: A unique identifier tag for the scene. Should be checked with isTagAvailable before creation.
		*/
		virtual void removeObject(Scene *scenePtr, IObjectFactory *factoryPtr, Object *objectPtr) = 0;

		/**
		* Returns true if the tag is available else false 
		*/
        virtual bool isTagAvailable(Scene *scenePtr, const std::string &tag) = 0;

		/**
		* Retrieves the object with the specified tag. Returns nullptr if it can't be retrieved.
		*/
        virtual Object *getObjectByTag(Scene *scenePtr, const std::string &tag) = 0;

		/**
		* Retrieves the list of object with the specified type.
		*/
        virtual std::vector<Object*> getObjectsByObjectType(Scene *scenePtr, const std::string &objectType) = 0;
    };
}

#endif //ANDROIDSDK_ISCENESERVICE_H
