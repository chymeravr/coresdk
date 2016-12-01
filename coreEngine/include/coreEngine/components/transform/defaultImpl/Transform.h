//
// Created by chimeralabs on 11/2/2016.
//

#ifndef ANDROIDSDK_TRANSFORM_H
#define ANDROIDSDK_TRANSFORM_H

#include <coreEngine/components/ITransform.h>
#include <coreEngine/service/IModelService.h>
#include <coreEngine/service/IMaterialService.h>
#include <coreEngine/service/IShaderService.h>
#include <coreEngine/service/ISceneService.h>
#include <coreEngine/model/Scene.h>

namespace cl{
    /**
     * Matrix that will be generated will have following matrix multiplication
     * T * Ry * Rx * Rz * S
     *
     */
    class Transform : public ITransform{
    private:
        Logger *loggerPtr;
        std::unique_ptr<IModelService> modelServicePtr;
        std::unique_ptr<IMaterialService> materialServicePtr;
        std::unique_ptr<IShaderService> shaderServicePtr;
        std::unique_ptr<ISceneService> sceneServicePtr;

        std::string type;
        Object *objectPtr;
        Scene *scenePtr;
        CL_Vec3 position;
        /**
         *  y>x>z hierarcy... Ry * Rx * Rz
         *  Adopted from Unity
         */
        CL_Vec3 rotation;
        CL_Vec3 scale;
    public:
        Transform();
        //IComponent Interface methods
        std::string getComponentType();
        void initialize();
        void draw();
        void deinitialize();
        void setObject(Object &object);
        Object &getObject();

        //ITransform interface methods
        void setScene(Scene &scene);
        void setPosition(CL_Vec3 position);
        CL_Vec3 getPosition();
        void setRoation(CL_Vec3 rotation);
        CL_Vec3 getRotation();
        void setScale(CL_Vec3 scale);
        CL_Vec3 getScale();
    };
}

#endif //ANDROIDSDK_TRANSFORM_H
