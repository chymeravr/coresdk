#ifndef COREENGINE_IRELATIONSTORE_H
#define COREENGINE_IRELATIONSTORE_H

#include <vector>
#include <coreEngine/renderObjects/IRelationable.h>

namespace cl{
    class IRelationStore {
    public:
        virtual std::vector<IRelationable*> getRelations(IRelationable *relationable, std::string targetRelationType) = 0;
        virtual void createRelation(IRelationable *relationable1, IRelationable *relationable2) = 0;
        virtual void destroyRelation(IRelationable *relationable1, IRelationable *relatinable2) = 0;
    };
}

#endif //COREENGINE_IRELATIONSTORE_H