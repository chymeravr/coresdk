#ifndef COREENGINE_SHADERUNIFORMCOLOR_H
#define COREENGINE_SHADERUNIFORMCOLOR_H

#include <coreEngine/renderObjects/Shader.h>

namespace cl
{
	class ShaderUniformColor : public Shader
	{
	public:
		virtual ~ShaderUniformColor() {}
		ShaderUniformColor(const std::string &sceneId, ILoggerFactory *loggerFactory, Scene *scene)
			: Shader(sceneId, loggerFactory, scene) {}
	};
}

#endif //COREENGINE_SHADERUNIFORMCOLOR_H