//
// Created by chimeralabs on 10/21/2016.
//

#ifndef ANDROIDSDK_SCENEFACTORY_H
#define ANDROIDSDK_SCENEFACTORY_H

#include <coreEngine/model/Scene.h>

namespace cl{
    class SceneFactory{
    public:
		/**
		* Creates a scene. Scene is to be stored by the owner and should be in scope all the time. If scene goes out of scope all other objects stored in the scene
		* goes out of scope.
		*/
		std::unique_ptr<Scene> createScene(std::unique_ptr<ISceneRenderer> sceneRendererPtr){
			return std::unique_ptr<Scene>(new Scene(std::move(sceneRendererPtr)));
		}

		/**
		* Deinitializes resources owned by scene. However it doesn't deallocate memory for Scene. Owner has to do the book-keeping or use smart_pointers
		*/
		void destroyScene(Object *objectPtr){
		}
    };
}

#endif //ANDROIDSDK_SCENEFACTORY_H
