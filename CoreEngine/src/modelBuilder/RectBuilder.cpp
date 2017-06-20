#include <coreEngine/modelBuilder/RectBuilder.h>

namespace cl{
	void RectBuilder::buildRect(Model *model, CL_GLfloat sideLength){
		std::vector<CL_Vec3> &vertices = model->getVertices();
		std::vector<CL_GLuint> &indices = model->getIndices();

		//front face
		vertices.push_back(CL_Vec3(-1.0, 1.0, -1.0));//left top front
		vertices.push_back(CL_Vec3(-1.0, -1.0, -1.0));//left bottom front
		vertices.push_back(CL_Vec3(1.0, -1.0, -1.0));//right bottom front
		vertices.push_back(CL_Vec3(1.0, 1.0, -1.0));//right top front

		/*
		Make rect of quads as it is easier to visualize.
		*/
		for (int i = 0; i < 4; i++){
			indices.push_back(i);
		}
	}
}