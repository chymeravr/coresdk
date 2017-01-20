#ifndef COREENGINETEST_RELATIONMOCK_H
#define COREENGINETEST_RELATIONMOCK_H

#include <gmock/gmock.h>
#include <coreEngine/renderObjects/Relation.h>

namespace cl{
	class RelationMock : public Relation{
	public:
		RelationMock(std::string id, std::string type, ILoggerFactory *loggerFactory) : Relation(loggerFactory){
			this->type = type;
			this->id = id;
		}
		std::string getType(){
			return type;
		}
		std::string getUniqueIdentifier(){
			return id;
		}
	private:
		std::string type;
		std::string id;
	};
}

#endif //COREENGINETEST_RELATIONMOCK_H