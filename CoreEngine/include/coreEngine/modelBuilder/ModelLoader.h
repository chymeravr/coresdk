#ifndef COREENGINE_MODELLOADER
#define COREENGINE_MODELLOADER

//#include <conf/GLTypes.h>
//#include <coreEngine/conf/MathType.h>
#include <coreEngine/renderObjects/Model.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
//#include <stdlib.h>
//#include <map>
//#include <vector>


namespace cl {
	class ModelLoader{
	public:
		static void load_obj(std::string filename, Model *model);

	private:
		static void processNode(aiNode *node, const aiScene *scene, Model *model);
		static void processMesh(aiMesh *mesh, Model *model);
	};
//class ModelLoader {
// public:
//  static void load_obj(std::string filename, Model *model);
//
// private:
//  static bool is_near(float v1, float v2);
//
//  // Searches through all already-exported vertices
//  // for a similar one.
//  // Similar = same position + same UVs + same normal
//  static bool getSimilarVertexIndex(CL_Vec3 &in_vertex, CL_Vec2 &in_uv,
//                                    CL_Vec3 &in_normal,
//                                    std::vector<CL_Vec3> &out_vertices,
//                                    std::vector<CL_Vec2> &out_uvs,
//                                    std::vector<CL_Vec3> &out_normals,
//                                    CL_GLuint &result);
//
//  static void indexVBO_slow(std::vector<CL_Vec3> &in_vertices,
//                            std::vector<CL_Vec2> &in_uvs,
//                            std::vector<CL_Vec3> &in_normals,
//
//                            std::vector<CL_GLuint> &out_indices,
//                            std::vector<CL_Vec3> &out_vertices,
//                            std::vector<CL_Vec2> &out_uvs,
//                            std::vector<CL_Vec3> &out_normals);
//
//  struct PackedVertex {
//    CL_Vec3 position;
//    CL_Vec2 uv;
//    CL_Vec3 normal;
//    bool operator<(const PackedVertex that) const {
//      return memcmp((void *)this, (void *)&that, sizeof(PackedVertex)) > 0;
//    };
//  };
//
//  static bool getSimilarVertexIndex_fast(
//      PackedVertex &packed, std::map<PackedVertex, CL_GLuint> &VertexToOutIndex,
//      CL_GLuint &result);
//
//  static void indexVBO(std::vector<CL_Vec3> &in_vertices,
//                       std::vector<CL_Vec2> &in_uvs,
//                       std::vector<CL_Vec3> &in_normals,
//
//                       std::vector<CL_GLuint> &out_indices,
//                       std::vector<CL_Vec3> &out_vertices,
//                       std::vector<CL_Vec2> &out_uvs,
//                       std::vector<CL_Vec3> &out_normals);
//
//  static void indexVBO_TBN(
//      std::vector<CL_Vec3> &in_vertices, std::vector<CL_Vec2> &in_uvs,
//      std::vector<CL_Vec3> &in_normals, std::vector<CL_Vec3> &in_tangents,
//      std::vector<CL_Vec3> &in_bitangents,
//
//      std::vector<CL_GLuint> &out_indices, std::vector<CL_Vec3> &out_vertices,
//      std::vector<CL_Vec2> &out_uvs, std::vector<CL_Vec3> &out_normals,
//      std::vector<CL_Vec3> &out_tangents, std::vector<CL_Vec3> &out_bitangents);
//};

}
#endif  // COREENGINE_MODELLOADER