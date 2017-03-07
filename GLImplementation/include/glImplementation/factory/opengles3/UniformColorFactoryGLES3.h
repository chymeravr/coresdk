#ifndef GLIMPLEMENTATION_UNIFORMCOLORFACTORYGLES3_H
#define GLIMPLEMENTATION_UNIFORMCOLORFACTORYGLES3_H

#include <coreEngine/factory/IUniformColorFactory.h>
#include <glImplementation/renderObjects/opengles3/ShaderUniformColorGLES3.h>
#include <glImplementation/renderObjects/opengles3/MaterialUniformColorGLES3.h>

namespace cl{
	class UniformColorFactoryGLES3 : public IUniformColorFactory{
	public:
		UniformColorFactoryGLES3(ILoggerFactory *loggerFactory);
		ShaderUniformColor *getShader(Scene *scene);
		MaterialUniformColor* getMaterial(ShaderUniformColor *shader, CL_Vec4 &color);

	private:
		ILoggerFactory *loggerFactory = nullptr;
	};
}

#endif //GLIMPLEMENTATION_UNIFORMCOLORFACTORYGLES3_H