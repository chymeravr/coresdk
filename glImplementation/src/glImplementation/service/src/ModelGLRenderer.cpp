//
// Created by chimeralabs on 10/24/2016.
//
#include <glImplementation/service/ModelGLRenderer.h>
#include <glImplementation/model/ShaderGL.h>
#include <glImplementation/service/IServiceGLPool.h>
#include <coreEngine/util/LoggerFactory.h>

namespace cl {

    ModelGLRenderer::ModelGLRenderer() {
        loggerPtr = &LoggerFactory::getInstance().getLoggerInstance(
                "app::glImplementation::ModelGLRenderer:");
        IServiceGLPool &serviceGLPool = IServiceGLPool::getInstance();
        componentServicePtr = serviceGLPool.getIComponentService();
        materialServicePtr = serviceGLPool.getIMaterialService();
        shaderGLServicePtr = serviceGLPool.getIShaderGLService();
        modelServicePtr = serviceGLPool.getIModelService();
    }

    bool ModelGLRenderer::initialize(Object &object) {
        std::vector<IComponent *> components = componentServicePtr->getComponents(object);
        for (auto it = components.cbegin(); it != components.cend(); it++) {
            (*it)->initialize();
        }
        ModelGL &model = (ModelGL &) object;

        if (model.getVertices().size() > 0) {
            createVertexBuffer(model);
        }
        if (model.getUvs().size() > 0) {
            createUVBuffer(model);
        }
        if (model.getColors().size() > 0) {
            createColorBuffer(model);
        }
        if (model.getNormals().size() > 0) {
            createNormalBuffer(model);
        }
        if (model.getIndices().size() > 0) {
            createIndexBuffer(model);
        }
        loggerPtr->log(LOG_DEBUG, "Initialized");
        return true;
    }

    bool ModelGLRenderer::draw(Object &object) {
        std::vector<IComponent *> components = componentServicePtr->getComponents(object);
        for (auto it = components.cbegin(); it != components.cend(); it++) {
            (*it)->draw();
        }
        ModelGL &modelGL = (ModelGL &) object;

        // ShaderGL &shaderGL = (ShaderGL&) materialPtr->getShader();
        // CL_GLuint mvpMatrixId = shaderGL.getCameraMVPMatrixUniformObject();

        //glUniformMatrix4fv(mvpMatrixId, 1, GL_FALSE, &cameraPtr->getMVPMatrix()[0][0]);

        std::pair<bool, Material &> materialPair = modelServicePtr->getMaterialOfModel(modelGL);
        if (materialPair.first) {
            std::pair<bool, Shader &> shaderPair = materialServicePtr->getShader(
                    materialPair.second);
            if (shaderPair.first) {
                std::pair<bool, Camera &> cameraPair = shaderGLServicePtr->getCamera((ShaderGL&)shaderPair.second);
                if (cameraPair.first) {
                    ShaderGL &shader = (ShaderGL&)shaderPair.second;
                    Camera &camera = cameraPair.second;
                    glUniformMatrix4fv(shader.getModelUniformObject(), 1, CL_GL_FALSE,
                                       &camera.getModelMatrix()[0][0]);
                    int i = 0;
                    if (glIsBuffer(modelGL.getVertexBuffer())) {
                        glEnableVertexAttribArray(i);
                        glBindBuffer(GL_ARRAY_BUFFER, modelGL.getVertexBuffer());
                        glVertexAttribPointer(
                                i,                                // attribute
                                3,                                // size
                                GL_FLOAT,                         // type
                                GL_FALSE,                         // normalized?
                                0,                                // stride
                                (void *) 0                          // array buffer offset
                        );
                        i++;
                    }
                    if (glIsBuffer(modelGL.getUvBuffer())) {
                        glEnableVertexAttribArray(i);
                        glBindBuffer(GL_ARRAY_BUFFER, modelGL.getUvBuffer());
                        glVertexAttribPointer(
                                i,                                // attribute
                                2,                                // size
                                GL_FLOAT,                         // type
                                GL_FALSE,                         // normalized?
                                0,                                // stride
                                (void *) 0                          // array buffer offset
                        );
                        i++;
                    }
                    if (glIsBuffer(modelGL.getColorBuffer())) {
                        glEnableVertexAttribArray(i);
                        glBindBuffer(GL_ARRAY_BUFFER, modelGL.getColorBuffer());
                        glVertexAttribPointer(
                                i,                                // attribute
                                4,                                // size
                                GL_FLOAT,                         // type
                                GL_FALSE,                         // normalized?
                                0,                                // stride
                                (void *) 0                          // array buffer offset
                        );
                        i++;
                    }
                    if (glIsBuffer(modelGL.getNormalBuffer())) {
                        glEnableVertexAttribArray(i);
                        glBindBuffer(GL_ARRAY_BUFFER, modelGL.getNormalBuffer());
                        glVertexAttribPointer(
                                i,                                // attribute
                                3,                                // size
                                GL_FLOAT,                         // type
                                GL_FALSE,                         // normalized?
                                0,                                // stride
                                (void *) 0                          // array buffer offset
                        );
                        i++;
                    }

                    if (glIsBuffer(modelGL.getIndexBuffer())) {
                        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelGL.getIndexBuffer());
                        glDrawElements(
                                GL_TRIANGLES,      // mode
                                modelGL.getIndices().size(),    // count
                                GL_UNSIGNED_SHORT,   // type
                                (void *) 0           // element array buffer offset
                        );
                    } else {
                        glDrawArrays(GL_TRIANGLES, 0, modelGL.getIndices().size());
                    }

                    i = 0;
                    if (glIsBuffer(modelGL.getVertexBuffer())) glDisableVertexAttribArray(i++);
                    if (glIsBuffer(modelGL.getUvBuffer())) glDisableVertexAttribArray(i++);
                    if (glIsBuffer(modelGL.getColorBuffer())) glDisableVertexAttribArray(i++);
                    if (glIsBuffer(modelGL.getNormalBuffer())) glDisableVertexAttribArray(i++);
                    loggerPtr->log(LOG_DEBUG, "Draw done.");

                }
            }
        }
        return true;
    }

    bool ModelGLRenderer::deinitialize(Object &object) {
        std::vector<IComponent *> components = componentServicePtr->getComponents(object);
        for (auto it = components.cbegin(); it != components.cend(); it++) {
            (*it)->deinitialize();
        }
        ModelGL &modelGL = (ModelGL &) object;
        if (glIsBuffer(modelGL.getVertexBuffer())) destroyVertexBuffer(modelGL);
        if (glIsBuffer(modelGL.getUvBuffer())) destroyUVBuffer(modelGL);
        if (glIsBuffer(modelGL.getColorBuffer())) destroyColorBuffer(modelGL);
        if (glIsBuffer(modelGL.getNormalBuffer())) destroyNormalBuffer(modelGL);
        if (glIsBuffer(modelGL.getIndexBuffer())) destroyIndexBuffer(modelGL);
        return true;
    }

    void ModelGLRenderer::createVertexBuffer(ModelGL &modelGL) {
        glGenBuffers(1, &modelGL.getVertexBuffer());
        glBindBuffer(GL_ARRAY_BUFFER, modelGL.getVertexBuffer());
        glBufferData(GL_ARRAY_BUFFER, modelGL.getVertices().size() * sizeof(CL_Vec3),
                     &modelGL.getVertices()[0],
                     (modelGL.getIsStatic()) ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
    }

    void ModelGLRenderer::useVertexBuffer(ModelGL &modelGL) {
        glBindBuffer(GL_ARRAY_BUFFER, modelGL.getVertexBuffer());
    }

    void ModelGLRenderer::destroyVertexBuffer(ModelGL &modelGL) {
        glDeleteBuffers(1, &modelGL.getVertexBuffer());
    }

    void ModelGLRenderer::createUVBuffer(ModelGL &modelGL) {
        glGenBuffers(1, &modelGL.getUvBuffer());
        glBindBuffer(GL_ARRAY_BUFFER, modelGL.getUvBuffer());
        glBufferData(GL_ARRAY_BUFFER, modelGL.getUvs().size() * sizeof(CL_Vec2),
                     &modelGL.getUvs()[0],
                     (modelGL.getIsStatic()) ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
    }

    void ModelGLRenderer::useUVBuffer(ModelGL &modelGL) {
        glBindBuffer(GL_ARRAY_BUFFER, modelGL.getUvBuffer());
    }

    void ModelGLRenderer::destroyUVBuffer(ModelGL &modelGL) {
        glDeleteBuffers(1, &modelGL.getUvBuffer());
    }

    void ModelGLRenderer::createColorBuffer(ModelGL &modelGL) {
        glGenBuffers(1, &modelGL.getColorBuffer());
        glBindBuffer(GL_ARRAY_BUFFER, modelGL.getColorBuffer());
        glBufferData(GL_ARRAY_BUFFER, modelGL.getColors().size() * sizeof(CL_Vec4),
                     &modelGL.getColors()[0],
                     (modelGL.getIsStatic()) ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
    }

    void ModelGLRenderer::useColorBuffer(ModelGL &modelGL) {
        glBindBuffer(GL_ARRAY_BUFFER, modelGL.getColorBuffer());
    }

    void ModelGLRenderer::destroyColorBuffer(ModelGL &modelGL) {
        glDeleteBuffers(1, &modelGL.getColorBuffer());
    }

    void ModelGLRenderer::createNormalBuffer(ModelGL &modelGL) {
        glGenBuffers(1, &modelGL.getNormalBuffer());
        glBindBuffer(GL_ARRAY_BUFFER, modelGL.getNormalBuffer());
        glBufferData(GL_ARRAY_BUFFER, modelGL.getNormals().size() * sizeof(CL_Vec3),
                     &modelGL.getNormals()[0],
                     (modelGL.getIsStatic()) ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
    }

    void ModelGLRenderer::useNormalBuffer(ModelGL &modelGL) {
        glBindBuffer(GL_ARRAY_BUFFER, modelGL.getNormalBuffer());
    }

    void ModelGLRenderer::destroyNormalBuffer(ModelGL &modelGL) {
        glDeleteBuffers(1, &modelGL.getNormalBuffer());
    }

    void ModelGLRenderer::createIndexBuffer(ModelGL &modelGL) {
        glGenBuffers(1, &modelGL.getIndexBuffer());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelGL.getIndexBuffer());
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, modelGL.getIndices().size() * sizeof(unsigned short),
                     &modelGL.getIndices()[0],
                     (modelGL.getIsStatic()) ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
    }

    void ModelGLRenderer::useIndexBuffer(ModelGL &modelGL) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelGL.getIndexBuffer());
    }

    void ModelGLRenderer::destroyIndexBuffer(ModelGL &modelGL) {
        glDeleteBuffers(1, &modelGL.getIndexBuffer());
    }

}