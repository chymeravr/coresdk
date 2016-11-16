//
// Created by chimeralabs on 10/24/2016.
//

#include <glImplementation/service/defaultImpl/ShaderGLService.h>
#include <glImplementation/service/IServiceGLPool.h>

namespace cl{

    ShaderGLService::ShaderGLService() {
        loggerPtr = &LoggerFactory::getInstance().getLoggerInstance("app::glImplementation::ShaderGLService:");
        objectServicePtr = IServiceGLPool::getInstance().getIObjectService();
    }

    void ShaderGLService::createShaderProgram(ShaderGL &shaderGL) {
        loggerPtr->log(LOG_DEBUG, "Compiling shader program for shader: " + shaderGL.getTag());
        CL_GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
        CL_GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

        GLint result = GL_FALSE;
        GLint infoLogLength;

        //compile vertex shader
        const char *vertexShaderSourcePtr = shaderGL.getVertexShaderCode().c_str();
        glShaderSource(vertexShaderId, 1, &vertexShaderSourcePtr , NULL);
        glCompileShader(vertexShaderId);

        //read vertex shader status
        glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &result);
        glGetShaderiv(vertexShaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
        if ( infoLogLength > 0 ){
            std::vector<char> vertexShaderErrorMessage(infoLogLength+1);
            glGetShaderInfoLog(vertexShaderId, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
            loggerPtr->log(LOG_INFO, std::string(&vertexShaderErrorMessage[0]));
        }

        //compile fragmentshader
        const char *fragmentShaderSourcePtr = shaderGL.getFragmentShaderCode().c_str();
        glShaderSource(fragmentShaderId, 1, &fragmentShaderSourcePtr , NULL);
        glCompileShader(fragmentShaderId);

        //read fragmentshader status
        glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &result);
        glGetShaderiv(fragmentShaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
        if ( infoLogLength > 0 ){
            std::vector<char> fragmentShaderErrorMessage(infoLogLength+1);
            glGetShaderInfoLog(fragmentShaderId, infoLogLength, NULL, &fragmentShaderErrorMessage[0]);
            loggerPtr->log(LOG_INFO, std::string(&fragmentShaderErrorMessage[0]));
        }

        //link the program
        CL_GLuint  programId = glCreateProgram();
        glAttachShader(programId, vertexShaderId);
        glAttachShader(programId, fragmentShaderId);
        glLinkProgram(programId);

        // Check the program
        glGetProgramiv(programId, GL_LINK_STATUS, &result);
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);
        if ( infoLogLength > 0 ){
            std::vector<char> programErrorMessage(infoLogLength+1);
            glGetProgramInfoLog(programId, infoLogLength, NULL, &programErrorMessage[0]);
            loggerPtr->log(LOG_INFO, std::string(&programErrorMessage[0]));
        }

        glDeleteShader(vertexShaderId);
        glDeleteShader(fragmentShaderId);
        shaderGL.setProgramId(programId);
        loggerPtr->log(LOG_INFO, "Finished compiling shader program for shader: " + shaderGL.getTag());
    }


    std::pair<bool, Camera &> ShaderGLService::getCamera(ShaderGL &shaderGL) {
        std::pair<bool, std::vector<Object *>& > camerasPair = objectServicePtr->getLinkedObjectsByObjectType(shaderGL, "camera");
        if(!camerasPair.first){
            return std::pair<bool, Camera&>(false, *(Camera*)NULL);
        }
        if(camerasPair.second.size() == 1){
            Object &object = *camerasPair.second[0];
            return std::pair<bool, Camera&>(true, (Camera&)object);
        }
        else{
            return std::pair<bool, Camera&>(false, *(Camera*)NULL);
        }
    }

    void ShaderGLService::setCamera(ShaderGL &shaderGL, Camera &camera) {
        std::pair<bool, Object&>  object = objectServicePtr->getLinkedObjectByTag(shaderGL, camera.getTag());
        if(!object.first){
            objectServicePtr->linkObject(shaderGL, camera);
            loggerPtr->log(LOG_INFO, "ShaderGL:" + shaderGL.getTag() + " added Camera:"+camera.getTag());
        }else{
            loggerPtr->log(LOG_WARN, "Material:" + shaderGL.getTag() + " already attached to Model:"+camera.getTag());
        }
    }

    void ShaderGLService::unsetCamera(ShaderGL &shaderGL, Camera &camera) {
        std::pair<bool, Object&>  object = objectServicePtr->getLinkedObjectByTag(shaderGL, camera.getTag());
        if(object.first){
            objectServicePtr->unlinkObject(shaderGL, camera);
            loggerPtr->log(LOG_WARN, "ShaderGL:" + shaderGL.getTag() + " detached from Camera:"+camera.getTag());
        }else{
            loggerPtr->log(LOG_INFO, "Camera:" + camera.getTag() + " not attached to ShaderGL:"+shaderGL.getTag()+". Hence, can't be removed.");
        }
    }
}