#ifndef GLESCONTEXTANDROID_GLES_CONTEXT_H
#define GLESCONTEXTANDROID_GLES_CONTEXT_H

#include <LoggerAndroidFactory.h>
#include <coreEngine/util/ILogger.h>

#include <sstream>
#include <iostream>
#include <GLES3/gl3.h>
#include <stdio.h>
#include <string>

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <coreEngine/util/ILoggerFactory.h>

class GLESNative {
public:
    GLESNative(cl::ILoggerFactory* loggerFactory, AAssetManager* mgr);
    void    PerformGLInits();
    void    Render();
    void    SetViewport(int width, int height);
    bool    IsInitsDone(){return initsDone;}
    std::string GetGLESVersionInfo() {return glesVersionInfo;}
    AAssetManager* getAAssetManager() {return this->mgr;}
private:
    bool    initsDone;
    std::string glesVersionInfo;
    AAssetManager* mgr;
    std::unique_ptr<cl::ILogger> logger;
};

#endif //GLESCONTEXTANDROID_GLES_CONTEXT_H
