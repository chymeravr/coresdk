#include <glImplementation/factory/opengles2/UniformColorFactoryGLES2.h>

#include <sstream>

namespace cl
{
// android version of std does not have a to_string
template <typename T>
std::string to_string(T value)
{
    std::ostringstream os;
    os << value;
    return os.str();
}

UniformColorFactoryGLES2::UniformColorFactoryGLES2(ILoggerFactory *loggerFactory)
{
    assert(loggerFactory != nullptr);
    this->loggerFactory = loggerFactory;
}
ShaderUniformColor *UniformColorFactoryGLES2::getShader(Scene *scene)
{
    std::string shaderId = "ShaderUniformColorGLES2";
    ShaderUniformColor *shader = nullptr;
    if (!(shader = (ShaderUniformColor *)scene->getFromScene(shaderId)))
    {
	//creating shader
	std::unique_ptr<ShaderUniformColor> shaderUptr(new ShaderUniformColorGLES2(shaderId, loggerFactory, scene));
	assert(shaderUptr != nullptr);
	shader = shaderUptr.get();
	scene->addToScene(std::move(shaderUptr));
    }
    return shader;
}
MaterialUniformColor *UniformColorFactoryGLES2::getMaterial(ShaderUniformColor *shader, CL_Vec4 &color)
{
    assert(shader != nullptr);
    std::vector<Relation *> sceneRelations = shader->getRelations("scene");
    assert(sceneRelations.size() == 1);
    Scene *scene = (Scene *)sceneRelations[0];
    std::string materialId = "MaterialUniformColor" + cl::to_string(color[0]) + cl::to_string(color[1]) + cl::to_string(color[2]) + cl::to_string(color[3]);
    MaterialUniformColor *material = nullptr;
    if (!(material = (MaterialUniformColor *)scene->getFromScene(materialId)))
    {
	//creating material
	std::unique_ptr<MaterialUniformColor> materialUptr(new MaterialUniformColorGLES2(materialId, (ShaderUniformColorGLES2 *)shader, loggerFactory, color));
	assert(materialUptr != nullptr);
	material = materialUptr.get();
	scene->addToScene(std::move(materialUptr));
    }
    return material;
}
}