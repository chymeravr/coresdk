#ifndef COREENGINE_SHADERTEXT_H
#define COREENGINE_SHADERTEXT_H

#include <coreEngine/renderObjects/Shader.h>

namespace cl{
	class ShaderText : public Shader{
	public:
		virtual ~ShaderText(){}
		ShaderText(const std::string &sceneId, 
				   ILoggerFactory *loggerFactory,
				   Scene *scene):Shader(sceneId, loggerFactory, scene){
		}
	};
}

#endif //COREENGINE_SHADERTEXT_H