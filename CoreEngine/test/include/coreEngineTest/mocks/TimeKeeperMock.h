#ifndef COREENGINETEST_TIMEKEEPERMOCK_H
#define COREENGINETEST_TIMEKEEPERMOCK_H

#include <coreEngine/animation/ITimeKeeper.h>
#include <gmock/gmock.h>

namespace cl {
	class TimeKeeperMock : public ITimeKeeper {
	public:
		TimeKeeperMock() : ITimeKeeper() {}
		~TimeKeeperMock(){}
	
  void start() {}
  float getElapsedTime() { return 0.5f; }
  /*MOCK_METHOD0(start, void());
  MOCK_METHOD0(getElapsedTime, float());*/
};
}

#endif  // COREENGINETEST_TIMEKEEPERMOCK_H