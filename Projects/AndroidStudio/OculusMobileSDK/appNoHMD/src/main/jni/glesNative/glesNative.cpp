#include "glesNative.h"


/**
 * Checks for OpenGL errors.Very useful while debugging. Call it as often as required
 */
void CheckGLError(std::string funcName, cl::ILogger *logger){

    GLenum err = glGetError();
    if (err == GL_NO_ERROR) {
        return;
    } else {
        std::string part1 = "[FAIL GL] ";
        std::string part2 = funcName;
        logger->log(cl::LOG_ERROR, part1 + funcName);
    }

    switch(err) {

        case GL_INVALID_ENUM:
            logger->log(cl::LOG_ERROR, "GL_INVALID_ENUM: GLenum argument out of range");
            break;

        case GL_INVALID_VALUE:
            logger->log(cl::LOG_ERROR, "GL_INVALID_VALUE: numeric argument out of range");
            break;

        case GL_INVALID_OPERATION:
            logger->log(cl::LOG_ERROR, "GL_INVALID_OPERATION: operation illegal in current state");
            break;

        case GL_INVALID_FRAMEBUFFER_OPERATION:
            logger->log(cl::LOG_ERROR, "GL_INVALID_FRAMEBUFFER_OPERATION: framebuffer object is not complete");
            break;

        case GL_OUT_OF_MEMORY:
            logger->log( cl::LOG_ERROR, "GL_OUT_OF_MEMORY: not enough memory left to execute command");
            break;

        default:
            logger->log(cl::LOG_ERROR, "unlisted error");
            break;
    }
}

/**
 * Class constructor
 */
GLESNative::GLESNative(cl::ILoggerFactory* loggerFactory, AAssetManager* mgr) {
    initsDone = false;
    glesVersionInfo ="";
    this->logger = loggerFactory->createLogger("");
    this->mgr = mgr;
}

/**
 * Perform inits and set various GLES options before rendering image
 */
void GLESNative::PerformGLInits() {

    // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // load the GLES version into a string
    std::ostringstream version_string;
    version_string << glGetString(GL_VERSION) << ", " << glGetString(GL_SHADING_LANGUAGE_VERSION);
    glesVersionInfo = std::string(version_string.str());

    // check if the device supports GLES 3 or GLES 2
    const char* versionStr = (const char*)glGetString(GL_VERSION);
    if (strstr(versionStr, "OpenGL ES 3.")) //&& gl3stubInit()) {
    {
        logger->log(cl::LOG_DEBUG, "Device supports GLES 3");
    } else {
        logger->log(cl::LOG_DEBUG, "Device supports GLES 2");
    }

    initsDone = true;

    CheckGLError("GLESNative::PerformGLInits", this->logger.get());
}


void GLESNative::Render() {

    // blue background
    glClearColor(0.0f, 0.0f, 1.0f, 0.0f);

    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    CheckGLError("GLESNative::Render", this->logger.get());

}

void GLESNative::SetViewport(int width, int height) {

    // set the viewport
    // function is also called when user changes device orientation
    glViewport(0, 0, width, height);

    CheckGLError("GLESNative::SetViewport", this->logger.get());
}
