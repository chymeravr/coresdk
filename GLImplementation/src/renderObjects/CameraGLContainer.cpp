#include <glImplementation/renderObjects/CameraGLContainer.h>

namespace cl{
    CameraGLContainer::CameraGLContainer(CL_GLuint &viewMatrixId, CL_GLuint &projectionMatrixId, CameraGL *cameraGL){
        this->viewMatrixId = viewMatrixId;
        this->projectionMatrixId = projectionMatrixId;
        this->cameraGL = cameraGL;
    }
    bool CameraGLContainer::initialize(){

    }
    void CameraGLContainer::draw(){
        glUniformMatrix4fv(viewMatrixId, 1, GL_FALSE, &(cameraGL->getViewMatrix())[0][0]);
        glUniformMatrix4fv(projectionMatrixId, 1, GL_FALSE, &(cameraGL->getProjectionMatrix())[0][0]);
    }
    void CameraGLContainer::deinitialize(){

    }
}