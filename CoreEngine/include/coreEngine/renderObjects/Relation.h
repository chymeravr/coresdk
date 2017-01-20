#ifndef COREENGINE_RELATION_H
#define COREENGINE_RELATION_H

#include <string>
#include <vector>
#include <unordered_map>
#include <coreEngine/util/ILoggerFactory.h>

namespace cl{
	class Relation{
	public:
		Relation(ILoggerFactory *loggerFactory);
		virtual ~Relation(){}
		virtual std::string getType() = 0;
		virtual std::string getUniqueIdentifier() = 0;
		void createRelation(Relation *relation);
		void createBiRelation(Relation *relation);
		std::vector<Relation*> getRelations(std::string targetRelationType);
		void destroyRelation(Relation *relation);
		void destroyBiRelation(Relation *relation);
		bool exists(Relation *relation);
	private:
		std::unique_ptr<ILogger> logger;
		std::unordered_map<std::string, std::vector<Relation*> > relationMap;
	};
}

#endif //COREENGINE_RELATION_H