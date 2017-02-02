#ifndef GLIMPLEMENTATION_UNIFORMCOLORFACTORYGL_H
#define GLIMPLEMENTATION_UNIFORMCOLORFACTORYGL_H

#include <coreEngine/factory/IUniformColorFactory.h>
#include <glImplementation/renderObjects/ShaderUniformColorGL.h>
#include <glImplementation/renderObjects/MaterialUniformColorGL.h>

namespace cl{
	class UniformColorFactoryGL : public IUniformColorFactory{
	public:
		std::unique_ptr<ShaderUniformColor> createShader(const std::string &sceneId, ILoggerFactory *loggerFactory, Scene *scene){
			return std::unique_ptr<ShaderUniformColor>(new ShaderUniformColorGL(sceneId, loggerFactory, scene));
		}
		std::unique_ptr<MaterialUniformColor> createMaterial(const std::string &sceneId, ShaderUniformColor *shader, ILoggerFactory *loggerFactory, CL_Vec4 &color){
			return std::unique_ptr<MaterialUniformColor>(new MaterialUniformColorGL(sceneId, (ShaderUniformColorGL*)shader, loggerFactory, color));
		}
	};
}

#endif //GLIMPLEMENTATION_UNIFORMCOLORFACTORYGL_H