#include <assert.h>
#include <coreEngine/modelBuilder/UVSphereBuilder.h>

namespace cl{
	UVSphereBuilder::UVSphereBuilder(ModelModifier *modelModifier){
		assert(modelModifier != nullptr);
		this->modelModifier = modelModifier;
	}

	void UVSphereBuilder::buildUnitSphere(Model *model, unsigned int nSubdivisionTimes){
		assert(model != nullptr);
		unsigned int extraRowsPerPole = 3;
		unsigned int nThetaDivisions = pow(2, nSubdivisionTimes + 2);
		unsigned int nPhiDivisions = pow(2, nSubdivisionTimes + 1);
		unsigned int totalPhis = nPhiDivisions + extraRowsPerPole * 2;//will make 3 extra rows at poles to minimize distortion

		std::vector<CL_Vec3> &vertices = model->getVertices();
		std::vector<CL_Vec2> &uvs = model->getUvs();
		std::vector<CL_GLuint> &indices = model->getIndices();
		// initializing the south pole latitude vertices
		float higherPhi = (CL_PI / 2);
		float lowerPhi = -higherPhi;
		float stepPhi = CL_PI / nPhiDivisions;

		float lowerTheta = 0.0f;
		float higherTheta = 2 * CL_PI;
		float stepTheta = higherTheta / nThetaDivisions;

		float currentPhi = lowerPhi;
		float currentTheta = lowerTheta;
		for (unsigned int iPhi = 0; iPhi <= totalPhis; iPhi++){
			float uvy;
			if (iPhi <= extraRowsPerPole){
				currentPhi = lowerPhi + iPhi*(stepPhi / (extraRowsPerPole + 1));
				//uvy = (iPhi / (extraRowsPerPole + 1.0f)) * (1.0f / nPhiDivisions);
				uvy = (iPhi / (extraRowsPerPole + 1.0f)) * ((vMax-vMin) / nPhiDivisions) + vMin;
			}
			else if (iPhi >= totalPhis - extraRowsPerPole){
				currentPhi = lowerPhi + (totalPhis - (2.0f * extraRowsPerPole + 1)) * stepPhi + (iPhi - (totalPhis - (extraRowsPerPole + 1.0f)))*(stepPhi / (extraRowsPerPole + 1.0f));
				float vMaxMinusMin = vMax-vMin;
				uvy = vMaxMinusMin - vMaxMinusMin / nPhiDivisions + (iPhi - (totalPhis - extraRowsPerPole - vMaxMinusMin)) / (extraRowsPerPole + vMaxMinusMin) * vMaxMinusMin / nPhiDivisions + vMin;
			}
			else{
				currentPhi = lowerPhi + (iPhi - extraRowsPerPole) * stepPhi;
				//uvy = ((iPhi - extraRowsPerPole) *1.0f) / nPhiDivisions;
				uvy = ((iPhi - extraRowsPerPole) *(vMax-vMin)) / nPhiDivisions + vMin;
			}
			currentTheta = lowerTheta;
			for (unsigned int iTheta = 0; iTheta <= nThetaDivisions; iTheta++){
				currentTheta = lowerTheta + iTheta * stepTheta;
				if (iTheta == nThetaDivisions){
					currentTheta = lowerTheta; //last vertex should be exactly same as first vertex of the row
				}
				float cosphi = cosf(currentPhi);
				vertices.push_back(CL_Vec3(cosphi*cosf(currentTheta), sinf(currentPhi), cosphi*sinf(currentTheta)));
				CL_Vec2 uv;
				if (iPhi == 0 || iPhi == totalPhis){
					uv.x = (uMax+uMin)/2; //Using the middle value of texture. Helpful in getting just one seam as opposed to one seam per triangle at poles
				}
				else if (iTheta == nThetaDivisions){
					uv.x = uMax;
				}
				else{
					uv.x = iTheta * (uMax-uMin) / nThetaDivisions + uMin;
				}
				uv.y = uvy;
				uvs.push_back(uv);
				if (iTheta == 0){
					continue;
				}
				if (iPhi == 0){
					continue;
				}

				unsigned int index1 = getIndex(iPhi - 1, iTheta - 1, nThetaDivisions);
				unsigned int index2 = getIndex(iPhi - 1, iTheta, nThetaDivisions);
				unsigned int index3 = getIndex(iPhi, iTheta, nThetaDivisions);
				unsigned int index4 = getIndex(iPhi, iTheta - 1, nThetaDivisions);
				indices.push_back(index1);
				indices.push_back(index2);
				indices.push_back(index3);
				indices.push_back(index4);
			}
		}
		modelModifier->convertQuadIndicesToTriangleIndices(model);
	}
	CL_Vec2 UVSphereBuilder::getUVMap(float longitude, float latitude){
		return CL_Vec2(longitude / (2 * CL_PI), (latitude + CL_PI / 2) / CL_PI);
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
		if (uv.x < 0){
			uv.x = 0.0f;
		}
		if (uv.x > 1){
			uv.x = 1.0f;
		}
		return uv;
	}

	unsigned int UVSphereBuilder::getIndex(unsigned int iPhi, unsigned int iTheta, unsigned int nThetaDivisions){
		return (nThetaDivisions + 1)*iPhi + iTheta;
	}

	CL_Vec3 UVSphereBuilder::getCartesianCoordinateFromThetaAndPhi(float phi, float theta){
		float y = sinf(phi);
		float cosphi = cosf(phi);
		/*if ((1 - cosphi) < 2 * epsilon){
			cosphi = 1 - 0.5*phi*phi;
			}*/
		float x = cosphi*cosf(theta);
		float z = cosphi*sinf(theta);
		return CL_Vec3(x, y, z);
	}
}