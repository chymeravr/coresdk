#include <coreEngine/conf/MathType.h>
#include <coreEngine/modelBuilder/ModelLoader.h>
#include <coreEngine/renderObjects/Model.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <vector>

namespace cl {
void ModelLoader::load_obj(std::string filename, Model *model) {
  std::vector<CL_Vec3> &vertices = model->getVertices();
  std::vector<CL_Vec2> &uvs = model->getUvs();
  std::vector<CL_Vec3> &normals = model->getNormals();
  std::vector<CL_GLuint> &elements = model->getIndices();

  std::vector<CL_Vec3> temp_vertices;
  std::vector<CL_Vec2> temp_uvs;
  std::vector<CL_Vec3> temp_normals;

  std::vector<CL_GLuint> vertexIndices, uvIndices, normalIndices;

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
      temp_vertices.push_back(vertex);
    } else if (strcmp(lineHeader, "vt") == 0) {
      isTextureCoordPresent = true;
      CL_Vec2 uv;
      fscanf(file, "%f %f\n", &uv.x, &uv.y);
      temp_uvs.push_back(uv);
    } else if (strcmp(lineHeader, "vn") == 0) {
      CL_Vec3 normal;
      fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
      temp_normals.push_back(normal);
    } else if (strcmp(lineHeader, "f") == 0) {
      // in general ~ a face can be any polygon, assuming a triangle for now
      std::string vertex1, vertex2, vertex3;
      // we can have differernt uv and normal indices ~ we will use only vertex
      // index for now
      unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
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
      vertexIndices.push_back(vertexIndex[0]);
      vertexIndices.push_back(vertexIndex[1]);
      vertexIndices.push_back(vertexIndex[2]);
      uvIndices.push_back(uvIndex[0]);
      uvIndices.push_back(uvIndex[1]);
      uvIndices.push_back(uvIndex[2]);
      normalIndices.push_back(normalIndex[0]);
      normalIndices.push_back(normalIndex[1]);
      normalIndices.push_back(normalIndex[2]);
    }
  }

  std::vector<CL_Vec3> all_vertices;
  std::vector<CL_Vec2> all_uvs;
  std::vector<CL_Vec3> all_normals;
  for (unsigned int i = 0; i < vertexIndices.size(); i++) {
    // Get the indices of its attributes
    unsigned int vertexIndex = vertexIndices[i];
    unsigned int normalIndex = normalIndices[i];
    unsigned int uvIndex = uvIndices[i];

    CL_Vec3 vertex = temp_vertices[vertexIndex - 1];
    CL_Vec3 normal = temp_normals[normalIndex - 1];
    CL_Vec2 uv = temp_uvs[uvIndex - 1];

    all_vertices.push_back(vertex);
    all_uvs.push_back(uv);
    all_normals.push_back(normal);
  }

  indexVBO(all_vertices, all_uvs, all_normals, elements, vertices, uvs,
           normals);
  fclose(file);
}

bool ModelLoader::getSimilarVertexIndex_fast(
    PackedVertex &packed, std::map<PackedVertex, CL_GLuint> &VertexToOutIndex,
    CL_GLuint &result) {
  std::map<PackedVertex, CL_GLuint>::iterator it =
      VertexToOutIndex.find(packed);
  if (it == VertexToOutIndex.end()) {
    return false;
  } else {
    result = it->second;
    return true;
  }
}

void ModelLoader::indexVBO(std::vector<CL_Vec3> &in_vertices,
                           std::vector<CL_Vec2> &in_uvs,
                           std::vector<CL_Vec3> &in_normals,

                           std::vector<CL_GLuint> &out_indices,
                           std::vector<CL_Vec3> &out_vertices,
                           std::vector<CL_Vec2> &out_uvs,
                           std::vector<CL_Vec3> &out_normals) {
  std::map<PackedVertex, CL_GLuint> VertexToOutIndex;

  // For each input vertex
  for (unsigned int i = 0; i < in_vertices.size(); i++) {
    PackedVertex packed = {in_vertices[i], in_uvs[i], in_normals[i]};

    // Try to find a similar vertex in out_XXXX
    CL_GLuint index;
    bool found = getSimilarVertexIndex_fast(packed, VertexToOutIndex, index);

    if (found) {  // A similar vertex is already in the VBO, use it instead !
      out_indices.push_back(index);
    } else {  // If not, it needs to be added in the output data.
      out_vertices.push_back(in_vertices[i]);
      out_uvs.push_back(in_uvs[i]);
      out_normals.push_back(in_normals[i]);
      CL_GLuint newindex = (CL_GLuint)out_vertices.size() - 1;
      out_indices.push_back(newindex);
      VertexToOutIndex[packed] = newindex;
    }
  }
}
}