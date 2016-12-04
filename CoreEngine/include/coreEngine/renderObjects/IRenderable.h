#ifndef COREENGINE_IRENDERABLE_H
#define COREENGINE_IRENDERABLE_H

namespace cl{
    class IRenderable{
    public:
        virtual bool initialize() = 0;
        virtual void draw() = 0;
        virtual void deinitialize() = 0;
    };
}

#endif COREENGINE_IRENDERABLE_H