#ifndef COREENGINETEST_GAZEDETECTORMOCK_H
#define COREENGINETEST_GAZEDETECTORMOCK_H

#include <gmock/gmock.h>
#include <coreEngine/components/gazeDetector/GazeDetector.h>

namespace cl{
	class GazeDetectorMock : public GazeDetector{
	public:
		GazeDetectorMock(std::string &idOfObjectComponentAttachedTo,
			TransformTree *shooterTransform,
			TransformTree *gazeObjectTransform,
			EventGazeListener *gazeListener,
			GazeDetectorContainer *gazeDetectorContainer) :GazeDetector(idOfObjectComponentAttachedTo, shooterTransform, gazeObjectTransform, gazeListener, gazeDetectorContainer)
		{}
		MOCK_METHOD0(getIsGazed, bool(void));
		MOCK_METHOD0(getDistanceFromShooter, float(void));
	};
}

#endif //COREENGINETEST_GAZEDETECTORMOCK_H