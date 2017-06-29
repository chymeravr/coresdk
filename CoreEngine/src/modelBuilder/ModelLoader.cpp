#include <coreEngine/conf/MathType.h>
#include <coreEngine/modelBuilder/ModelLoader.h>
#include <coreEngine/renderObjects/Model.h>
#include <vector>
#include <stdlib.h>
#include <map>
#include <iostream>


namespace cl {
	void ModelLoader::load_obj(std::string filename, Model *model) {
		Assimp::Importer import;
		const aiScene *scene = import.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);

		std::cout << "Input FileName : " << filename << std::endl;
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
			return;
		}
		auto directory = filename.substr(0, filename.find_last_of('/'));

		processNode(scene->mRootNode, scene, model);
	}

	void ModelLoader::processNode(aiNode *node, const aiScene *scene, Model *model)
	{
		// process all the node's meshes (if any)
		std::cout << "Number of Meshes : " << node->mNumMeshes << std::endl;
		std::cout << "Number of Nodes : " << node->mNumChildren << std::endl;
		//for (unsigned int i = 0; i < node->mNumMeshes; i++)
		//{
		//	std::cout << "Processing a Node's Mesh" << std::endl;
		//	aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		//	//meshes.push_back(processMesh(mesh, scene));
		//}
		//// then do the same for each of its children
		//for (unsigned int i = 0; i < node->mNumChildren; i++)
		//{
		//	processNode(node->mChildren[i], scene, model);
		//}

		// assimp nodes can have hierarchical relationships amongst different nodes etc. 
		// we will implicitly assume only one node exists for each object we load. 
		// our intent is to load a controller rendering while writing this code
		assert(node->mNumMeshes == 0);
		assert(node->mNumChildren == 1);
		auto childNode = node->mChildren[0];
		//assert(childNode->mNumMeshes == 1);
		auto childMesh = scene->mMeshes[childNode->mMeshes[0]];
		processMesh(childMesh, model);
	}

	void ModelLoader::processMesh(aiMesh *mesh, Model *model)
	{
		std::cout << "Processing Mesh" << std::endl;
		// data to fill
		//vector<Vertex> vertices;
		//vector<unsigned int> indices;
		//vector<Texture> textures;
		
		std::vector<CL_Vec3> &vertices = model->getVertices();
		std::vector<CL_Vec2> &uvs = model->getUvs();
		std::vector<CL_Vec3> &normals = model->getNormals();
		std::vector<CL_GLuint> &indices = model->getIndices();

		std::cout << "Number of Vertices in this mesh : " << mesh->mNumVertices << std::endl;
		std::cout << "Number of UVs in this mesh : " << mesh->mNumUVComponents << std::endl;
		std::cout << "Number of Faces in this mesh : " << mesh->mNumFaces << std::endl;
		// Walk through each of the mesh's vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			CL_Vec3 vertex;
			CL_Vec2 uv;
			CL_Vec3 normal;
			
			// positions
			vertex.x = mesh->mVertices[i].x;
			vertex.y = mesh->mVertices[i].y;
			vertex.z = mesh->mVertices[i].z;
			vertices.push_back(vertex);
			// normals
			normal.x = mesh->mNormals[i].x;
			normal.y = mesh->mNormals[i].y;
			normal.z = mesh->mNormals[i].z;
			normals.push_back(normal);
			// texture coordinates
			// currently single texture per vertex ~ we need this to change. TODO ~ SUPPORT MULTIPLE TEXTURE COORDINATES
			if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				uv.x = mesh->mTextureCoords[0][i].x;
				uv.y = mesh->mTextureCoords[0][i].y;

				uvs.push_back(uv);
			}
			
		}
		// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// retrieve all indices of the face and store them in the indices vector
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		
	}
//void ModelLoader::load_obj(std::string filename, Model *model) {
//  std::vector<CL_Vec3> &vertices = model->getVertices();
//  std::vector<CL_Vec2> &uvs = model->getUvs();
//  std::vector<CL_Vec3> &normals = model->getNormals();
//  std::vector<CL_GLuint> &elements = model->getIndices();
//
//  std::vector<CL_Vec3> temp_vertices;
//  std::vector<CL_Vec2> temp_uvs;
//  std::vector<CL_Vec3> temp_normals;
//
//  std::vector<CL_GLuint> vertexIndices, uvIndices, normalIndices;
//
//  FILE *file = fopen(filename.c_str(), "r");
//  if (file == NULL) {
//    // todo ~ logger print
//    printf("Impossible to open the file !\n");
//    return;
//  }
//
//  bool isTextureCoordPresent = false;
//  while (1) {
//    char lineHeader[128];
//    // read the first word of the line
//    int res = fscanf(file, "%s", lineHeader);
//    if (res == EOF) {
//      break;  // EOF = End Of File. Quit the loop.
//    }
//    // else : parse lineHeader
//    if (strcmp(lineHeader, "v") == 0) {
//      CL_Vec3 vertex;
//      fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
//      temp_vertices.push_back(vertex);
//    } else if (strcmp(lineHeader, "vt") == 0) {
//      isTextureCoordPresent = true;
//      CL_Vec2 uv;
//      fscanf(file, "%f %f\n", &uv.x, &uv.y);
//      temp_uvs.push_back(uv);
//    } else if (strcmp(lineHeader, "vn") == 0) {
//      CL_Vec3 normal;
//      fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
//      temp_normals.push_back(normal);
//    } else if (strcmp(lineHeader, "f") == 0) {
//      // in general ~ a face can be any polygon, assuming a triangle for now
//      std::string vertex1, vertex2, vertex3;
//      // we can have differernt uv and normal indices ~ we will use only vertex
//      // index for now
//      unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
//      if (isTextureCoordPresent) {
//        int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
//                             &vertexIndex[0], &uvIndex[0], &normalIndex[0],
//                             &vertexIndex[1], &uvIndex[1], &normalIndex[1],
//                             &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
//        if (matches != 9) {
//          printf(
//              "File can't be read by our simple parser : ( Try exporting with "
//              "other options\n");
//          return;
//        }
//        vertexIndices.push_back(vertexIndex[0]);
//        vertexIndices.push_back(vertexIndex[1]);
//        vertexIndices.push_back(vertexIndex[2]);
//        uvIndices.push_back(uvIndex[0]);
//        uvIndices.push_back(uvIndex[1]);
//        uvIndices.push_back(uvIndex[2]);
//        normalIndices.push_back(normalIndex[0]);
//        normalIndices.push_back(normalIndex[1]);
//        normalIndices.push_back(normalIndex[2]);
//
//      } else {
//        int matches = fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0],
//                             &normalIndex[0], &vertexIndex[1], &normalIndex[1],
//                             &vertexIndex[2], &normalIndex[2]);
//
//        if (matches != 6) {
//          printf(
//              "File can't be read by our simple parser : ( Try exporting with "
//              "other options\n");
//          return;
//        }
//        vertexIndices.push_back(vertexIndex[0]);
//        vertexIndices.push_back(vertexIndex[1]);
//        vertexIndices.push_back(vertexIndex[2]);
//        normalIndices.push_back(normalIndex[0]);
//        normalIndices.push_back(normalIndex[1]);
//        normalIndices.push_back(normalIndex[2]);
//      }
//      //  elements.push_back(vertexIndex[0]);
//      //  elements.push_back(vertexIndex[1]);
//      //  elements.push_back(vertexIndex[2]);
//    }
//  }
//
//  printf("Processing indices\n");
//  // For each vertex of each triangle
//  std::vector<glm::vec3> out_vertices;
//  std::vector<glm::vec2> out_uvs;
//  std::vector<glm::vec3> out_normals;
//  for (unsigned int i = 0; i < vertexIndices.size(); i++) {
//    // Get the indices of its attributes
//    unsigned int vertexIndex = vertexIndices[i];
//    unsigned int normalIndex = normalIndices[i];
//
//    // Get the attributes thanks to the index
//    CL_Vec3 vertex = temp_vertices[vertexIndex - 1];
//    CL_Vec3 normal = temp_normals[normalIndex - 1];
//
//    // Put the attributes in buffers
//    out_vertices.push_back(vertex);
//    out_normals.push_back(normal);
//
//    if (isTextureCoordPresent) {
//      unsigned int uvIndex = uvIndices[i];
//      CL_Vec2 uv = temp_uvs[uvIndex - 1];
//      uvs.push_back(uv);
//    }
//  }
//  indexVBO_slow(out_vertices, out_uvs, out_normals, elements, vertices, uvs,
//                normals);
//  fclose(file);
//}
//
//bool ModelLoader::is_near(float v1, float v2) { return fabs(v1 - v2) < 0.01f; }
//
//// Searches through all already-exported vertices
//// for a similar one.
//// Similar = same position + same UVs + same normal
//bool ModelLoader::getSimilarVertexIndex(CL_Vec3 &in_vertex, CL_Vec2 &in_uv,
//                                        CL_Vec3 &in_normal,
//                                        std::vector<CL_Vec3> &out_vertices,
//                                        std::vector<CL_Vec2> &out_uvs,
//                                        std::vector<CL_Vec3> &out_normals,
//                                        CL_GLuint &result) {
//  printf("getSimilarVertexIndex");
//  // Lame linear search
//  for (unsigned int i = 0; i < out_vertices.size(); i++) {
//    if (is_near(in_vertex.x, out_vertices[i].x) &&
//        is_near(in_vertex.y, out_vertices[i].y) &&
//        is_near(in_vertex.z, out_vertices[i].z) &&
//        is_near(in_uv.x, out_uvs[i].x) && is_near(in_uv.y, out_uvs[i].y) &&
//        is_near(in_normal.x, out_normals[i].x) &&
//        is_near(in_normal.y, out_normals[i].y) &&
//        is_near(in_normal.z, out_normals[i].z)) {
//      result = i;
//      return true;
//    }
//  }
//  // No other vertex could be used instead.
//  // Looks like we'll have to add it to the VBO.
//  return false;
//}
//
//void ModelLoader::indexVBO_slow(std::vector<CL_Vec3> &in_vertices,
//                                std::vector<CL_Vec2> &in_uvs,
//                                std::vector<CL_Vec3> &in_normals,
//
//                                std::vector<CL_GLuint> &out_indices,
//                                std::vector<CL_Vec3> &out_vertices,
//                                std::vector<CL_Vec2> &out_uvs,
//                                std::vector<CL_Vec3> &out_normals) {
//  // For each input vertex
//  printf("Indexing");
//  for (unsigned int i = 0; i < in_vertices.size(); i++) {
//    // Try to find a similar vertex in out_XXXX
//    CL_GLuint index;
//    bool found =
//        getSimilarVertexIndex(in_vertices[i], in_uvs[i], in_normals[i],
//                              out_vertices, out_uvs, out_normals, index);
//    printf("Index : %d", index);
//    if (found) {  // A similar vertex is already in the VBO, use it instead !
//      out_indices.push_back(index);
//    } else {  // If not, it needs to be added in the output data.
//      out_vertices.push_back(in_vertices[i]);
//      out_uvs.push_back(in_uvs[i]);
//      out_normals.push_back(in_normals[i]);
//      out_indices.push_back((CL_GLuint)out_vertices.size() - 1);
//    }
//  }
//}
//
//bool ModelLoader::getSimilarVertexIndex_fast(
//    PackedVertex &packed, std::map<PackedVertex, CL_GLuint> &VertexToOutIndex,
//    CL_GLuint &result) {
//  std::map<PackedVertex, CL_GLuint>::iterator it =
//      VertexToOutIndex.find(packed);
//  if (it == VertexToOutIndex.end()) {
//    return false;
//  } else {
//    result = it->second;
//    return true;
//  }
//}
//
//void ModelLoader::indexVBO(std::vector<CL_Vec3> &in_vertices,
//                           std::vector<CL_Vec2> &in_uvs,
//                           std::vector<CL_Vec3> &in_normals,
//
//                           std::vector<CL_GLuint> &out_indices,
//                           std::vector<CL_Vec3> &out_vertices,
//                           std::vector<CL_Vec2> &out_uvs,
//                           std::vector<CL_Vec3> &out_normals) {
//  std::map<PackedVertex, CL_GLuint> VertexToOutIndex;
//
//  // For each input vertex
//  for (unsigned int i = 0; i < in_vertices.size(); i++) {
//    PackedVertex packed = {in_vertices[i], in_uvs[i], in_normals[i]};
//
//    // Try to find a similar vertex in out_XXXX
//    CL_GLuint index;
//    bool found = getSimilarVertexIndex_fast(packed, VertexToOutIndex, index);
//
//    if (found) {  // A similar vertex is already in the VBO, use it instead !
//      out_indices.push_back(index);
//    } else {  // If not, it needs to be added in the output data.
//      out_vertices.push_back(in_vertices[i]);
//      out_uvs.push_back(in_uvs[i]);
//      out_normals.push_back(in_normals[i]);
//      CL_GLuint newindex = (CL_GLuint)out_vertices.size() - 1;
//      out_indices.push_back(newindex);
//      VertexToOutIndex[packed] = newindex;
//    }
//  }
//}
//
//void ModelLoader::indexVBO_TBN(
//    std::vector<CL_Vec3> &in_vertices, std::vector<CL_Vec2> &in_uvs,
//    std::vector<CL_Vec3> &in_normals, std::vector<CL_Vec3> &in_tangents,
//    std::vector<CL_Vec3> &in_bitangents,
//
//    std::vector<CL_GLuint> &out_indices, std::vector<CL_Vec3> &out_vertices,
//    std::vector<CL_Vec2> &out_uvs, std::vector<CL_Vec3> &out_normals,
//    std::vector<CL_Vec3> &out_tangents, std::vector<CL_Vec3> &out_bitangents) {
//  // For each input vertex
//  for (unsigned int i = 0; i < in_vertices.size(); i++) {
//    // Try to find a similar vertex in out_XXXX
//    CL_GLuint index;
//    bool found =
//        getSimilarVertexIndex(in_vertices[i], in_uvs[i], in_normals[i],
//                              out_vertices, out_uvs, out_normals, index);
//
//    if (found) {  // A similar vertex is already in the VBO, use it instead !
//      out_indices.push_back(index);
//
//      // Average the tangents and the bitangents
//      out_tangents[index] += in_tangents[i];
//      out_bitangents[index] += in_bitangents[i];
//    } else {  // If not, it needs to be added in the output data.
//      out_vertices.push_back(in_vertices[i]);
//      out_uvs.push_back(in_uvs[i]);
//      out_normals.push_back(in_normals[i]);
//      out_tangents.push_back(in_tangents[i]);
//      out_bitangents.push_back(in_bitangents[i]);
//      out_indices.push_back((CL_GLuint)out_vertices.size() - 1);
//    }
//  }
//}
}