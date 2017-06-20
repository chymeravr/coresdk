#ifndef COREENGINE_RECTBUILDER_H
#define COREENGINE_RECTBUILDER_H

#include <coreEngine/renderObjects/Model.h>
#include <coreEngine/modifier/ModelModifier.h>

namespace cl{
	class RectBuilder{
	public:
        /**
		* Builds a Rectangle
		*/
        void buildRect(Model *model, CL_GLfloat sideLength);
	};
}

#endif //COREENGINE_RECTBUILDER_H