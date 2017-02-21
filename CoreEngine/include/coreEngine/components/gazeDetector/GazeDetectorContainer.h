#ifndef COREENGINE_GAZEDETECTORCONTAINER_H
#define COREENGINE_GAZEDETECTORCONTAINER_H

#include <unordered_map>

namespace cl{
	class GazeDetector;
	class GazeDetectorContainer{
	public:
		unsigned int totalGazeDetectors = 0;
		unsigned int gazeDetectorsScannedInCurrentLoop = 0;
		std::unordered_map<std::string, GazeDetector*> gazeDetectors;
		GazeDetector *currentTarget = nullptr;
	};
}

#endif //COREENGINE_GAZEDETECTORCONTAINER_H