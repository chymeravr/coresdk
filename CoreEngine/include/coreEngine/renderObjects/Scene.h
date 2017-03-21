#ifndef COREENGIN_SCENE_H
#define COREENGIN_SCENE_H

#include <memory>
#include <unordered_map>
#include <coreEngine/conf/Types.h>
#include <coreEngine/renderObjects/IRenderable.h>
#include <coreEngine/renderObjects/Relation.h>
#include <coreEngine/renderObjects/IScenable.h>
#include <coreEngine/util/ILoggerFactory.h>

namespace cl{
	class Scene : public Relation{
	public:
		Scene(ILoggerFactory *loggerFactory, std::string id);
		std::string getType();
		std::string getUniqueIdentifier();
		virtual IRenderable *getRenderable() = 0;
		virtual void setBackgroundColor(CL_Vec4 color) = 0;
		virtual void setDepthTest(bool enable) = 0;
		virtual void setBlending(bool enable) = 0;
		/**
		* Add To Scene will add a Scenable object to the Scene. It should be provided with unique sceneId else it will abort.
		*/
		virtual void addToScene(std::unique_ptr<IScenable> scenable);
		virtual IScenable *getFromScene(std::string sceneId);
		virtual void removeFromScene(IScenable *scenable);
		virtual bool exists(std::string sceneId);

	private:
		std::string id = "";
		const std::string type = "scene";
		std::unique_ptr<ILogger> logger;
		std::unordered_map<std::string, std::unique_ptr<IScenable> > scenableMap;
	};
}

#endif //COREENGIN_SCENE_H