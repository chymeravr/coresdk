//
// Created by chimeralabs on 10/18/2016.
//

#ifndef ANDROIDSDK_SHADER_H
#define ANDROIDSDK_SHADER_H

#include <unordered_map>
#include <coreEngine/model/Object.h>
#include <coreEngine/model/IShaderRenderer.h>

namespace cl{
    class Shader : public Object{
    protected:
        std::unique_ptr<IShaderRenderer> shaderRendererPtr;
    public:
        Shader(std::unique_ptr<IShaderRenderer> shaderRendererPtr, const std::string &tag) : Object("shader", tag){
            this->shaderRendererPtr = std::move(shaderRendererPtr);
        }

        /**
         * Returns renderer to the object. No setter for this as it is set in the constructor
         */
        IShaderRenderer *getShaderRendererPtr(){
            return shaderRendererPtr.get();
        }
    };

}

#endif //ANDROIDSDK_SHADER_H
