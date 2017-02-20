#ifndef COREENGINE_GAZEDETECTORFACTORY_H
#define COREENGINE_GAZEDETECTORFACTORY_H

#include <coreEngine/components/gazeDetector/GazeDetectorBox.h>
#include <coreEngine/components/gazeDetector/GazeDetectorContainer.h>

namespace cl{
	class GazeDetectorFactory{
	public:
		std::unique_ptr<IComponent> createGazeDetectorBox(std::string &idOfObjectComponentAttachedTo,
			TransformTree *shooterTransform,
			TransformTree *gazeObjectTransform,
			EventGazeListener *gazeListener,
			GazeDetectorContainer *gazeDetectorContainer,
			CL_Vec3 &shooterLocalOrigin,
			CL_Vec3 &shooterLocalLookAt,
			float lengthX,
			float lengthY,
			float lengthZ){

			return std::unique_ptr<IComponent>(new GazeDetectorBox(idOfObjectComponentAttachedTo, shooterTransform, gazeObjectTransform, gazeListener, gazeDetectorContainer, shooterLocalOrigin, shooterLocalLookAt, lengthX, lengthY, lengthZ));
		}

		std::unique_ptr<GazeDetectorContainer> createGazeDetectorContainer(){
			return std::unique_ptr<GazeDetectorContainer>(new GazeDetectorContainer);
		}
	};
}

#endif //COREENGINE_GAZEDETECTORFACTORY_H