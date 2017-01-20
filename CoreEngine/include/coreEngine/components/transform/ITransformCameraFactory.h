#ifndef COREENGINE_ITRANSFORMCAMERAFACTORY_H
#define COREENGINE_ITRANSFORMCAMERAFACTORY_H

#include <memory>
#include <coreEngine/components/transform/TransformCamera.h>

namespace cl{
	class ITransformCameraFactory{
	public:
		virtual std::unique_ptr<TransformCamera> create(Camera *camera) = 0;
	};
}

#endif //COREENGINE_ITRANSFORMCAMERAFACTORY_H