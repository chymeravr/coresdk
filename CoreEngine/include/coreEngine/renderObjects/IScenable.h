#ifndef COREENGINE_ISCENABLE
#define COREENGINE_ISCENABLE

#include <string>

namespace cl{
	class IScenable {
	public:
		virtual std::string getSceneId() = 0;
	};
}

#endif //COREENGINE_ISCENABLE