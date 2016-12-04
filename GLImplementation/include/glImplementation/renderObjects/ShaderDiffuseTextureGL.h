#ifndef GLIMPLEMENTATION_SHADERDIFFUSETEXTUREGL_H
#define GLIMPLEMENTATION_SHADERDIFFUSETEXTUREGL_H

#include <coreEngine/renderObjects/ShaderDiffuseTexture.h>
#include <coreEngine/renderObjects/IRenderable.h>

namespace cl{
    class ShaderDiffuseTextureGL : public ShaderDiffuseTexture, public IRenderable{
        ShaderDiffuseTextureGL(const std::string &sceneId, IRelationStore *relationStore);
        IRenderable *getRenderable();
        /*
        * Here in initialize, set the program id to all those who need it (Model, Material etc.)
        */
        bool initialize();
        void draw();
        void deinitialize();
    };
}

#endif //GLIMPLEMENTATION_SHADERDIFFUSETEXTUREGL_H