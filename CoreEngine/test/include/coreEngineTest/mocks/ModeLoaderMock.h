#ifndef COREENGINETEST_MODELLOADERMOCK_H
#define COREENGINETEST_MODELLOADERMOCK_H

#include <gmock/gmock.h>
#include <coreEngine/modelBuilder/ModelLoader.h>

namespace cl{
	class ModelLoaderMock : public ModelLoader{
	public:
		ModelLoaderMock() :ModelLoader(){}
	};
}

#endif //COREENGINETEST_MODELLOADERMOCK_H