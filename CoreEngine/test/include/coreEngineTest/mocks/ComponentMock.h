#ifndef COREENGINETEST_COMPONENTMOCK_H
#define COREENGINETEST_COMPONENTMOCK_H

#include <gmock/gmock.h>
#include <coreEngine/renderObjects/IComponent.h>

namespace cl{
	class ComponentMock : public IComponent{
	public:
		std::string getComponentType(){
			return "componentMock";
		}
		MOCK_METHOD0(getRenderable, IRenderable*(void));
	};
}

#endif //COREENGINETEST_COMPONENTMOCK_H