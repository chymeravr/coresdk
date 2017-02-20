#ifndef COREENGINE_CONCENTRICCIRCLEBUILDER_H
#define COREENGINE_CONCENTRICCIRCLEBUILDER_H

#include <coreEngine/renderObjects/Model.h>

namespace cl{
	class ConcentricCircleBuilder{
	public:
		void buildConcentricCircle(Model *model, float rInner, float rOuter, unsigned int nDivisions = 20);
	};
}

#endif //COREENGINE_CONCENTRICCIRCLEBUILDER_H