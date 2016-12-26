#ifndef COREENGINE_UVSPHEREBUILDER_H
#define COREENGINE_UVSPHEREBUILDER_H

#include <coreEngine/renderObjects/Model.h>
#include <coreEngine/modifier/ModelModifier.h>

namespace cl{
    class UVSphereBuilder{
    public:
        UVSphereBuilder(ModelModifier *modelModifier);
        /*
        * Creates a unit sphere.
        * Number of triangles used in the sphere is 16*2^(2*nSubdivisionTimes). Here nSubdivisionTimes = 0 is the base case in which 
        * there are 8 quadrilaterals in total (16 triangles). Also, here the poles are considered as latitude with epsilon distance
        * from actual 90 degre poles.
        * @arg nSubdivisionTimes: number of times sphere will be divided. >=0
        */
        virtual void buildUnitSphere(Model *model, unsigned int nSubdivisionTimes);
        virtual CL_Vec2 getUVMap(CL_Vec3 vertex);
        virtual CL_Vec2 getUVMap(float longitude, float latitude);

    private:
        CL_Vec3 getCartesianCoordinateFromThetaAndPhi(float phi, float theta);
        unsigned int getIndex(unsigned int iPhi, unsigned int iTheta, unsigned int nThetaDivisions);
        
        ModelModifier *modelModifier = nullptr;
        float epsilon = 0.000001f;
    };
}

#endif //UVCOREENGINE_SPHEREBUILDER_H