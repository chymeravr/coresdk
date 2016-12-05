#ifndef COREENGINE_IAPPLICATION_H
#define COREENGINE_IAPPLICATION_H

namespace cl{
    class IApplication{
    public:
        virtual void start() = 0;
        virtual void initialize() = 0;
        virtual void update() = 0;
        virtual void draw() = 0;
        virtual void deinitialize() = 0;
        virtual void stop() = 0;
    };
}

#endif //COREENGINE_IAPPLICATION_H
