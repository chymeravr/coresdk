#ifndef COREENGINE_IRELATIONABLE_H
#define COREENGINE_IRELATIONABLE_H

#include <string>

namespace cl{
    class IRelationable{
    public:
        virtual std::string getType() = 0;
    };
}

#endif //COREENGINE_IRELATIONABLE_H