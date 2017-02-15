#ifndef COREENGINE_CHARACTER_H
#define COREENGINE_CHARACTER_H

#include <coreEngine/conf/MathType.h>
#include <coreEngine/conf/Types.h>
#include <coreEngine/renderObjects/Texture.h>

namespace cl{
	class Character{
	public:
		Character(CL_Vec2 size, CL_Vec2 bearing, CL_GLuint advance){
			this->size = size;
			this->bearing = bearing;
			this->advance = advance;
		}
		CL_Vec2 size;
		CL_Vec2 bearing;
		CL_GLuint advance;
	};
}

#endif //COREENGINE_CHARACTER_H