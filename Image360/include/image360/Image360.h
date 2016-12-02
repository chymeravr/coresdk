#ifndef IMAGE360_IMAGE360_H
#define IMAGE360_IMAGE360_H

#include<memory>
#include<coreEngine/IRenderer.h>

namespace cl{
    class Image360{
    public:
        Image360(std::unique_ptr<IRenderer> renderer);
    private:
        std::unique_ptr<IRenderer> renderer;
    };
}

#endif //IMAGE360_IMAGE360_H