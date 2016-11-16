//
// Created by chimeralabs on 10/19/2016.
//

#ifndef ANDROIDSDK_SHADERGL_H
#define ANDROIDSDK_SHADERGL_H

#include <string>
#include <coreEngine/model/Shader.h>
#include <conf/GLTypes.h>

namespace cl{
    class ShaderGL : public Shader{
    protected:
        std::string vertexShaderCode = "";
        std::string fragmentShaderCode = "";
        CL_GLuint programId;
        CL_GLuint modelUniformObject;
        CL_GLuint viewUniformObject;
        CL_GLuint projectionUniformObject;
        Camera *cameraPtr;
    public:
        ShaderGL(IObjectRenderer &objectRenderer, const std::string &tag) : Shader(objectRenderer,
                                                                                   tag) { }

        void setVertexShaderCode(std::string &code){
            vertexShaderCode = code;
        }
        const std::string &getVertexShaderCode(){
            return vertexShaderCode;
        }
        void setFragmentShaderCode(std::string &code){
            fragmentShaderCode = code;
        }
        const std::string &getFragmentShaderCode(){
            return fragmentShaderCode;
        }
        void setProgramId(CL_GLuint &programId) {
            this->programId = programId;
        }
        CL_GLuint &getProgramId(){
            return programId;
        }
        CL_GLuint getModelUniformObject() const {
            return modelUniformObject;
        }
        void setModelUniformObject(CL_GLuint modelUniformObject) {
            ShaderGL::modelUniformObject = modelUniformObject;
        }
        CL_GLuint getViewUniformObject() const {
            return viewUniformObject;
        }
        void setViewUniformObject(CL_GLuint viewUniformObject) {
            ShaderGL::viewUniformObject = viewUniformObject;
        }
        CL_GLuint getProjectionUniformObject() const {
            return projectionUniformObject;
        }
        void setProjectionUniformObject(CL_GLuint projectionUniformObject) {
            ShaderGL::projectionUniformObject = projectionUniformObject;
        }
        void setCamera(Camera &camera){
            this->cameraPtr = &camera;
        }
        Camera &getCamera(){
            return *this->cameraPtr;
        }
        virtual void createMVPUniformObjects() = 0;
    };
}

#endif //ANDROIDSDK_SHADERGL_H
