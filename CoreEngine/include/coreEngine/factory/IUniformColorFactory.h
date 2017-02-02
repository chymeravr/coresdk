#ifndef CORENEGINE_IUNIFORMCOLORFACTORY_H
#define CORENEGINE_IUNIFORMCOLORFACTORY_H

#include <memory>
#include <coreEngine/renderObjects/MaterialUniformColor.h>
#include <coreEngine/renderObjects/ShaderUniformColor.h>

namespace cl{
	class IUniformColorFactory{
	public:
		virtual ~IUniformColorFactory(){}
		virtual std::unique_ptr<ShaderUniformColor> createShader(const std::string &sceneId, ILoggerFactory *loggerFactory, Scene *scene) = 0;
		virtual std::unique_ptr<MaterialUniformColor> createMaterial(const std::string &sceneId, ShaderUniformColor *shader, ILoggerFactory *loggerFactory,CL_Vec4 &color) = 0;
	};
}

#endif //CORENEGINE_IUNIFORMCOLORFACTORY_H