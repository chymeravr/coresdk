#ifndef COREENGINE_MODELMODIFIER_H
#define COREENGINE_MODELMODIFIER_H

#include <coreEngine/renderObjects/Model.h>

namespace cl{
    class ModelModifier{
    public:
        virtual void convertQuadIndicesToTriangleIndices(Model *model);
        /**
        * For each triangle in indices, consider vertices 1, 2, 3. If original triangle is 1-2-3 invertNormal makes it 1-3-2
        */
        virtual void invertNormal(Model *model);
    };
}

#endif //COREENGINE_MODELMODIFIER_H