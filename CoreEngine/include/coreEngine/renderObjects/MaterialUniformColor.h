#ifndef COREENGINE_MATERIALUNIFORMCOLOR_H
#define COREENGINE_MATERIALUNIFORMCOLOR_H

#include <coreEngine/renderObjects/Material.h>
#include <coreEngine/renderObjects/Texture.h>
#include <coreEngine/renderObjects/ShaderUniformColor.h>

namespace cl{
	class MaterialUniformColor : public Material{
	public:
		virtual ~MaterialUniformColor(){}
		MaterialUniformColor(const std::string &sceneId, ShaderUniformColor  *shader, ILoggerFactory *loggerFactory, CL_Vec4 &color) : Material(sceneId, shader, loggerFactory){
			this->color = color;
		}
		void setColor(CL_Vec4 &color){
			this->color = color;
		}
		CL_Vec4 &getColor(){
			return this->color;
		}
	protected:
		CL_Vec4 color;
	};
}

#endif //COREENGINE_MATERIALUNIFORMCOLOR_H