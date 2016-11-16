//
// Created by chimeralabs on 10/25/2016.
//

#include <glImplementation/service/ShaderGLUniformColorRenderer.h>
#include <coreEngine/util/LoggerFactory.h>
#include <glImplementation/service/IServiceGLPool.h>
#include <glImplementation/model/ShaderGLUniformColor.h>

namespace cl{

    ShaderGLUniformColorRenderer::ShaderGLUniformColorRenderer() {
        loggerPtr = &LoggerFactory::getInstance().getLoggerInstance("app::glImplementation::ShaderGLUniformColorRenderer:");
        IServiceGLPool &serviceGLPool = IServiceGLPool::getInstance();
        shaderGLServicePtr = serviceGLPool.getIShaderGLService();
        sceneServicePtr = serviceGLPool.getISceneService();
        componentServicePtr = serviceGLPool.getIComponentService();
    }

    bool ShaderGLUniformColorRenderer::initialize(Object &object) {
        std::vector<IComponent *> components = componentServicePtr->getComponents(object);
        for(auto it = components.cbegin(); it!=components.cend(); it++){
            (*it)->initialize();
        }
        ShaderGLUniformColor &shaderGLUniformColor = (ShaderGLUniformColor&) object;
        shaderGLServicePtr->createShaderProgram(shaderGLUniformColor);
        shaderGLUniformColor.createMVPUniformObjects();

        loggerPtr->log(LOG_DEBUG, "Initialized");
        return true;
    }

    bool ShaderGLUniformColorRenderer::draw(Object &object) {
        std::vector<IComponent *> components = componentServicePtr->getComponents(object);
        for(auto it = components.cbegin(); it!=components.cend(); it++){
            (*it)->draw();
        }
        loggerPtr->log(LOG_DEBUG, "component drawn");
        ShaderGLUniformColor &shaderGLUniformColor = (ShaderGLUniformColor&) object;
        std::pair<bool, Camera&> cameraPair = shaderGLServicePtr->getCamera(shaderGLUniformColor);
        if(!cameraPair.first){
            loggerPtr->log(LOG_ERROR, "camera couldn't be retrieved from shader:"+object.getTag());
        }
        Camera &camera = cameraPair.second;
        std::string tag = camera.getTag();
        glUseProgram(shaderGLUniformColor.getProgramId());
        //std::vector<IComponent *> cameraComponents = componentServicePtr->getComponents(camera);
        //for(auto it = cameraComponents.cbegin(); it != cameraComponents.cend(); it++){
        //   (*it)->draw();
        //}
        //glUniformMatrix4fv(shaderGLUniformColor.getModelUniformObject(), 1, CL_GL_FALSE, &camera.getModelMatrix()[0][0]);
        //glUniformMatrix4fv(shaderGLUniformColor.getViewUniformObject(), 1, CL_GL_FALSE, &camera.getViewMatrix()[0][0]);
        //glUniformMatrix4fv(shaderGLUniformColor.getProjectionUniformObject(), 1, CL_GL_FALSE, &camera.getProjectionMatrix()[0][0]);

        loggerPtr->log(LOG_DEBUG, "Draw done.");
        return true;
    }

    bool ShaderGLUniformColorRenderer::deinitialize(Object &object) {
        std::vector<IComponent *> components = componentServicePtr->getComponents(object);
        for(auto it = components.cbegin(); it!=components.cend(); it++){
            (*it)->deinitialize();
        }
        glDeleteProgram(((ShaderGL&)object).getProgramId());
        return true;
    }


}