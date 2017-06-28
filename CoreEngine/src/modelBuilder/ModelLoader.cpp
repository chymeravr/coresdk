#include <coreEngine/conf/MathType.h>
#include <coreEngine/modelBuilder/ModelLoader.h>
#include <coreEngine/renderObjects/Model.h>
#include <vector>

namespace cl {
void ModelLoader::load_obj(std::string filename, Model *model) {
  std::vector<CL_Vec3> &vertices = model->getVertices();
  std::vector<CL_Vec2> &uvs = model->getUvs();
  std::vector<CL_Vec3> &normals = model->getNormals();
  std::vector<CL_GLuint> &elements = model->getIndices();

  FILE *file = fopen(filename.c_str(), "r");
  if (file == NULL) {
    // todo ~ logger print
    printf("Impossible to open the file !\n");
    return;
  }

  bool isTextureCoordPresent = false;
  while (1) {
    char lineHeader[128];
    // read the first word of the line
    int res = fscanf(file, "%s", lineHeader);
    if (res == EOF) {
      break;  // EOF = End Of File. Quit the loop.
    }
    // else : parse lineHeader
    if (strcmp(lineHeader, "v") == 0) {
      CL_Vec3 vertex;
      fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
      vertices.push_back(vertex);
    } else if (strcmp(lineHeader, "vt") == 0) {
      isTextureCoordPresent = true;
      CL_Vec2 uv;
      fscanf(file, "%f %f\n", &uv.x, &uv.y);
      uvs.push_back(uv);
    } else if (strcmp(lineHeader, "vn") == 0) {
      glm::vec3 normal;
      fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
      normals.push_back(normal);
    } else if (strcmp(lineHeader, "f") == 0) {
      // in general ~ a face can be any polygon, assuming a triangle for now
      std::string vertex1, vertex2, vertex3;
      // we can have differernt uv and normal indices ~ we will use only vertex
      // index for now
      unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
      if (isTextureCoordPresent) {
        int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
                             &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                             &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                             &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
        if (matches != 9) {
          printf(
              "File can't be read by our simple parser : ( Try exporting with "
              "other options\n");
          return;
        }
      } else {
        int matches = fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0],
                             &normalIndex[0], &vertexIndex[1], &normalIndex[1],
                             &vertexIndex[2], &normalIndex[2]);

        if (matches != 6) {
          printf(
              "File can't be read by our simple parser : ( Try exporting with "
              "other options\n");
          return;
        }
      }
      elements.push_back(vertexIndex[0]);
      elements.push_back(vertexIndex[1]);
      elements.push_back(vertexIndex[2]);
    }
  }
}
}