#include <glImplementation/factory/UniformColorFactoryGL.h>

// to_string is not implemented in android - hence different implementations
namespace cl{
	
	UniformColorFactoryGL::UniformColorFactoryGL(ILoggerFactory *loggerFactory){
		assert(loggerFactory != nullptr);
		this->loggerFactory = loggerFactory;
	}
	ShaderUniformColor *UniformColorFactoryGL::getShader(Scene *scene){
		std::string shaderId = "ShaderUniformColorGL";
		ShaderUniformColor *shader = nullptr;
		if (!(shader = (ShaderUniformColor*)scene->getFromScene(shaderId))){
			//creating shader
			std::unique_ptr<ShaderUniformColor> shaderUptr(new ShaderUniformColorGL(shaderId, loggerFactory, scene));
			assert(shaderUptr != nullptr);
			shader = shaderUptr.get();
			scene->addToScene(std::move(shaderUptr));
		}
		return shader;
	}
	MaterialUniformColor* UniformColorFactoryGL::getMaterial(ShaderUniformColor *shader, CL_Vec4 &color){
		assert(shader != nullptr);
		std::vector<Relation*> sceneRelations = shader->getRelations("scene");
		assert(sceneRelations.size() == 1);
		Scene *scene = (Scene*)sceneRelations[0];
		std::string materialId = "MaterialUniformColor" + std::to_string(color[0]) + std::to_string(color[1]) + std::to_string(color[2]) + std::to_string(color[3]);
		MaterialUniformColor *material = nullptr;
		if (!(material = (MaterialUniformColor*)scene->getFromScene(materialId))){
			//creating material
			std::unique_ptr<MaterialUniformColor> materialUptr(new MaterialUniformColorGL(materialId, (ShaderUniformColorGL*)shader, loggerFactory, color));
			assert(materialUptr != nullptr);
			material = materialUptr.get();
			scene->addToScene(std::move(materialUptr));
		}
		return material;
	}
}