#ifndef GLIMPLEMENTATION_MATERIALDIFFUSETEXTUREGL_H
#define GLIMPLEMENTATION_MATERIALDIFFUSETEXTUREGL_H

#include <coreEngine/renderObjects/MaterialDiffuseTexture.h>
#include <coreEngine/renderObjects/IRenderable.h>
#include <conf/GLTypes.h>
#include <glImplementation/renderObjects/ShaderDiffuseTextureGL.h>

namespace cl{
    class MaterialDiffuseTextureGL : public MaterialDiffuseTexture, public IRenderable{
    public:
        MaterialDiffuseTextureGL(const std::string &sceneId, IRelationStore *relationStore, ShaderDiffuseTextureGL *shader);
        IRenderable *getRenderable();
        void setDiffuseTextureId(std::unique_ptr<CL_GLuint> textureUnitId);//To be set by respective shader
        bool initialize();
        void draw();
        void deinitialize();
    };
}

#endif //GLIMPLEMENTATION_MATERIALDIFFUSETEXTUREGL_H