#ifndef GLIMPLEMENTATION_UNIFORMCOLORFACTORYGL_H
#define GLIMPLEMENTATION_UNIFORMCOLORFACTORYGL_H

#include <coreEngine/factory/IUniformColorFactory.h>
#include <glImplementation/renderObjects/ShaderUniformColorGL.h>
#include <glImplementation/renderObjects/MaterialUniformColorGL.h>

namespace cl{
	class UniformColorFactoryGL : public IUniformColorFactory{
	public:
		UniformColorFactoryGL(ILoggerFactory *loggerFactory);
		ShaderUniformColor *getShader(Scene *scene);
		MaterialUniformColor* getMaterial(ShaderUniformColor *shader, CL_Vec4 &color);

	private:
		ILoggerFactory *loggerFactory = nullptr;
	};
}

#endif //GLIMPLEMENTATION_UNIFORMCOLORFACTORYGL_H