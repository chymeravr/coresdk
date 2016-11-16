//
// Created by chimeralabs on 10/19/2016.
//

#ifndef ANDROIDSDK_MODELGL_H
#define ANDROIDSDK_MODELGL_H

#include <coreEngine/model/Model.h>
#include <conf/GLTypes.h>

namespace cl{
    class ModelGL : public Model{
        CL_GLuint vertexBuffer;
        CL_GLuint uvBuffer;
        CL_GLuint colorBuffer;
        CL_GLuint normalBuffer;
        CL_GLuint indexBuffer;
    public:

        ModelGL(std::string &tag, IObjectRenderer &objectRenderer) : Model(tag, objectRenderer) { }

        CL_GLuint &getVertexBuffer() {
            return vertexBuffer;
        }

        void setVertexBuffer(CL_GLuint &vertexBuffer) {
            ModelGL::vertexBuffer = vertexBuffer;
        }

        CL_GLuint &getUvBuffer() {
            return uvBuffer;
        }

        void setUvBuffer(CL_GLuint &uvBuffer) {
            ModelGL::uvBuffer = uvBuffer;
        }

        CL_GLuint &getColorBuffer() {
            return colorBuffer;
        }

        void setColorBuffer(CL_GLuint &colorBuffer) {
            ModelGL::colorBuffer = colorBuffer;
        }

        CL_GLuint &getNormalBuffer() {
            return normalBuffer;
        }

        void setNormalBuffer(CL_GLuint &normalBuffer) {
            ModelGL::normalBuffer = normalBuffer;
        }

        CL_GLuint &getIndexBuffer() {
            return indexBuffer;
        }

        void setIndexBuffer(CL_GLuint &indexBuffer) {
            ModelGL::indexBuffer = indexBuffer;
        }
    };
}

#endif //ANDROIDSDK_MODELGL_H
