#ifndef COREENGINE_MATERIAL_H
#define COREENGINE_MATERIAL_H

#include <coreEngine/renderObjects/Relation.h>
#include <coreEngine/renderObjects/IScenable.h>
#include <coreEngine/renderObjects/Shader.h>
#include <coreEngine/renderObjects/IRenderable.h>

namespace cl{
	class Material : public Relation, public IScenable{
	public:
		virtual ~Material(){}
		Material(const std::string &sceneId, Shader *shader, ILoggerFactory *loggerFactory) : Relation(loggerFactory){
			this->sceneId = sceneId;
			this->createBiRelation(shader);
		}
		virtual IRenderable *getRenderable() = 0;
		std::string getType(){
			return this->type;
		}
		std::string getUniqueIdentifier(){
			return this->sceneId;
		}
		std::string getSceneId(){
			return this->sceneId;
		}

	protected:
		std::string sceneId = "";
		const std::string type = "material";
	};
}

#endif //COREENGINE_MATERIAL_H