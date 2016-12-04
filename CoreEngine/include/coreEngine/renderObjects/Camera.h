#ifndef COREENGINE_CAMERA_H
#define COREENGINE_CAMERA_H

#include <coreEngine/renderObjects/IScenable.h>
#include <coreEngine/conf/Types.h>
#include <coreEngine/renderObjects/IRenderable.h>

namespace cl{
    class Camera : public IScenable {
    public:
        Camera(const std::string &sceneId);
        std::string getSceneId();
        virtual IRenderable *getRenderable() = 0;
        virtual void calculateViewMatrix();
        virtual void calculateProjectionMatrix();
        CL_Mat44 getViewMatrix();
        CL_Mat44 getProjectionMatrx();
        void setLocation(const CL_Vec3 &location);
        void setLookAtPoint(const CL_Vec3 &lookAtPoint);
        void setUpVector(const CL_Vec3 &upVector);
        /**
        * fov is in radians. It represents vertical field of view from top to down.
        */
        void setFov(const float &fov);

        /**
        * aspect ratio = width/height
        */
        void setAspect(const float &aspect);
        void setNearPlane(const float &nearPlane);
        void setFarPlane(const float &farPlane);

    protected:
        CL_Vec3 location;
        CL_Vec3 lookAtPoint;
        CL_Vec3 upVector;
        /**
        * fov is in radians. It represents vertical field of view from top to down.
        */
        float fov;
        /**
        * aspect ratio = width/height
        */
        float aspect;
        float nearPlane;
        float farPlane;
        CL_Vec3 localLocation = CL_Vec3(0.0, 0.0, 0.0);
        CL_Vec3 localLookAtPoint = CL_Vec3(0.0, 0.0, -1.0);
        CL_Vec3 localUpVector = CL_Vec3(0.0, 1.0, 0.0);
    };
}

#endif //COREENGINE_CAMERA_H