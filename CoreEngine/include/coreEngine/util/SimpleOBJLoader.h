#ifndef COREENGINE_SIMPLEOBJLOADER
#define COREENGINE_SIMPLEOBJLOADER

#include <string>
#include <coreEngine/renderObjects/Model.h>

namespace cl{
	class SimpleOBJLoader{
	public:
		static bool load(std::string filePath, Model *model);
	};
}

#endif //COREENGINE_SIMPLEOBJLOADER