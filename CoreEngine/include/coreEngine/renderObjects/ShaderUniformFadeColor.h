#ifndef COREENGINE_SHADERUNIFORMFADECOLOR_H
#define COREENGINE_SHADERUNIFORMFADECOLOR_H

#include <coreEngine/renderObjects/Shader.h>

namespace cl
{
	class ShaderUniformFadeColor : public Shader
	{
	public:
		virtual ~ShaderUniformFadeColor() {}
		ShaderUniformFadeColor(const std::string &sceneId, 
            ILoggerFactory *loggerFactory, Scene *scene)
			: Shader(sceneId, loggerFactory, scene) {}
	};
}

#endif //COREENGINE_SHADERUNIFORMFADECOLOR_H