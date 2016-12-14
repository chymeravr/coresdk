#include <coreEngine/renderObjects/Relation.h>
#include <assert.h>

namespace cl{
    Relation::Relation(ILoggerFactory *loggerFactory){
        assert(loggerFactory != nullptr);
        this->logger = loggerFactory->createLogger("CoreEngine::Relation: ");
    }

    void Relation::createRelation(Relation *relation){
        assert(relation != nullptr);
        assert(!exists(relation));
        relationMap[relation->getType()].push_back(relation);
    }

    bool Relation::exists(Relation *relation){
        assert(relation != nullptr);
        std::string type = relation->getType();
        if (relationMap.find(type) == relationMap.cend()){
            return false;
        }
        else{
            std::vector<Relation*> &relations = relationMap[type];
            for (auto it = relations.cbegin(); it != relations.cend(); it++){
                if ((*it)->getUniqueIdentifier() == relation->getUniqueIdentifier()){
                    return true;
                }
            }
            return false;
        }
    }

    std::vector<Relation*> Relation::getRelations(std::string targetRelationType){
        if (relationMap.find(targetRelationType) == relationMap.cend()){
            return std::vector<Relation*>();
        }
        else{
            return relationMap[targetRelationType];
        }
    }

    void Relation::destroyRelation(Relation *relation){
        assert(relation != nullptr);
        assert(exists(relation));
        std::vector<Relation*> &relations = relationMap[relation->getType()];
        for (auto it = relations.begin(); it != relations.end(); it++){
            if ((*it)->getUniqueIdentifier() == relation->getUniqueIdentifier()){
                Relation *swap = relations.back();
                (*it) = swap;
                relations.pop_back();
                return;
            }
        }
    }

    void Relation::createBiRelation(Relation *relation){
        assert(relation != nullptr);
        assert(!exists(relation));
        assert(!relation->exists(this));
        relation->createRelation(this);
        this->createRelation(relation);
    }

    void Relation::destroyBiRelation(Relation *relation){
        assert(relation != nullptr);
        assert(exists(relation));
        assert(relation->exists(this));
        relation->destroyRelation(this);
        this->destroyRelation(relation);
    }
}