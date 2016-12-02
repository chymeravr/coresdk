#ifndef COREENGINE_IRENDERER_H
#define COREENGINE_IRENDERER_H

namespace cl{
    class IRenderer{
    public:
        virtual bool start() = 0;
        virtual bool initialize() = 0;
        virtual bool update() = 0;
        virtual bool draw() = 0;
        virtual bool deinitialize() = 0;
        virtual bool stop() = 0;
    };
}

#endif //COREENGINE_IRENDERER_H