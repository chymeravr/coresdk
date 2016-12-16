#include <assert.h>
#include <coreEngine/modelBuilder/UVSphereBuilder.h>

namespace cl{
    UVSphereBuilder::UVSphereBuilder(ModelModifier *modelModifier){
        assert(modelModifier != nullptr);
        this->modelModifier = modelModifier;
    }

    void UVSphereBuilder::buildUnitSphere(Model *model, unsigned int nSubdivisionTimes){
        assert(model != nullptr);
        // phi for latitudes - range from -(pi/2 - epsilon) to (pi/2 - epsilon)
        // theta for longitudes - range from (epsion) to (2pi - epsilon)
        // nSubdivisionTimes = 0 is the base case when there is only one equator and poles and four longitudes representing an octahedron
        unsigned int nThetaDivisions = pow(2, nSubdivisionTimes + 2);
        unsigned int nPhiDivisions = pow(2, nSubdivisionTimes + 1);

        std::vector<CL_Vec3> &vertices = model->getVertices();
        std::vector<CL_GLuint> &indices = model->getIndices();
        // initializing the south pole latitude vertices
        float higherPhi = (CL_PI / 2 - epsilon);
        float lowerPhi = -higherPhi;
        float stepPhi = (higherPhi - lowerPhi) / nPhiDivisions;

        float lowerTheta = epsilon;
        float higherTheta = 2 * CL_PI - epsilon;
        float stepTheta = (higherTheta - lowerTheta) / nThetaDivisions;

        float currentPhi = lowerPhi;
        float currentTheta = lowerTheta;
        for (unsigned int iTheta = 0; iTheta <= nThetaDivisions; iTheta++){
            currentTheta = lowerTheta + iTheta * stepTheta;
            vertices.push_back(getCartesianCoordinateFromThetaAndPhi(currentPhi, currentTheta));
        }
        for (unsigned int iPhi = 1; iPhi <= nPhiDivisions; iPhi++){
            currentTheta = lowerTheta;
            currentPhi = lowerPhi + iPhi*stepPhi;
            vertices.push_back(getCartesianCoordinateFromThetaAndPhi(currentPhi, currentTheta));
            for (unsigned int iTheta = 1; iTheta <= nThetaDivisions; iTheta++){
                currentTheta = lowerTheta + iTheta * stepTheta;
                vertices.push_back(getCartesianCoordinateFromThetaAndPhi(currentPhi, currentTheta));
                unsigned int index1 = getIndex(iPhi - 1, iTheta - 1, nThetaDivisions);
                unsigned int index2 = getIndex(iPhi - 1, iTheta, nThetaDivisions);
                unsigned int index3 = getIndex(iPhi, iTheta - 1, nThetaDivisions);
                unsigned int index4 = getIndex(iPhi, iTheta, nThetaDivisions);
                indices.push_back(index1);
                indices.push_back(index2);
                indices.push_back(index3);
                indices.push_back(index4);
            }
        }
        modelModifier->convertQuadIndicesToTriangleIndices(model);
    }

    CL_Vec2 UVSphereBuilder::getUVMap(CL_Vec3 vertex){
        CL_Vec2 uv;
        float phi = asin(vertex.y);
        uv.y = (phi + CL_PI / 2) / CL_PI;
        if (uv.y < 0){
            uv.y = 0.0f;
        }
        if (uv.y > 1){
            uv.y = 1.0f;
        }
        float theta;
        float y2 = vertex.y * vertex.y;
        if ((1 - y2) < epsilon){
            float planarDistance = sqrt(vertex.x * vertex.x + vertex.z * vertex.z);
            if (planarDistance == 0){
                theta = -CL_PI / 2;
            }
            else if (planarDistance < epsilon){
                theta = vertex.z / planarDistance;
            }
            else{
                theta = asin(vertex.z / planarDistance);
            }
        }
        else{
            float sinTheta = vertex.z / sqrt(1 - y2);
            if (sinTheta > 1) //checking if due to numerical error argument is not out of bound
                sinTheta = 1;
            if (sinTheta < -1)
                sinTheta = -1;
            theta = asin(sinTheta);
        }

        if (vertex.x >= 0 && vertex.z >= 0){
            theta = theta;
        }
        else if (vertex.x < 0){
            theta = CL_PI - theta;
        }
        else{
            theta = 2 * CL_PI + theta;
        }
        uv.x = theta / (2 * CL_PI);
        if (uv.x< 0){
            uv.x = 0.0f;
        }
        if (uv.x > 1){
            uv.x = 1.0f;
        }
        return uv;
    }

    void UVSphereBuilder::generateUVMapForAllVertices(Model *model){
        assert(model != nullptr);
        std::vector<CL_Vec3> &vertices = model->getVertices();
        std::vector<CL_Vec2> &uvs = model->getUvs();
        uvs.clear();
        for (auto it = vertices.cbegin(); it != vertices.cend(); it++){
            uvs.push_back(getUVMap(*it));
        }
    }

    unsigned int UVSphereBuilder::getIndex(unsigned int iPhi, unsigned int iTheta, unsigned int nThetaDivisions){
        return (nThetaDivisions + 1)*iPhi + iTheta;
    }

    CL_Vec3 UVSphereBuilder::getCartesianCoordinateFromThetaAndPhi(float phi, float theta){
        float y = sin(phi);
        float x = cos(phi)*cos(theta);
        float z = cos(phi)*sin(theta);
        return CL_Vec3(x, y, z);
    }
}