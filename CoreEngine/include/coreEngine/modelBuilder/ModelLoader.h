#ifndef COREENGINE_MODELLOADER
#define COREENGINE_MODELLOADER

#include <coreEngine/renderObjects/Model.h>
#include <coreEngine/util/ILogger.h>
#include <map>

namespace cl {

struct PackedVertex {
  CL_Vec3 position;
  CL_Vec2 uv;
  CL_Vec3 normal;
  bool operator<(const PackedVertex that) const {
    return memcmp((void *)this, (void *)&that, sizeof(PackedVertex)) > 0;
  };
};

class ModelLoader {
 public:
  ModelLoader(ILoggerFactory *loggerFactory);
  ~ModelLoader(){};
  void load_obj(std::string filename, Model *model);

 private:
  bool getSimilarVertexIndex_fast(
      PackedVertex &packed, std::map<PackedVertex, CL_GLuint> &VertexToOutIndex,
      CL_GLuint &result);
  void indexVBO(std::vector<CL_Vec3> &in_vertices, std::vector<CL_Vec2> &in_uvs,
                std::vector<CL_Vec3> &in_normals,

                std::vector<CL_GLuint> &out_indices,
                std::vector<CL_Vec3> &out_vertices,
                std::vector<CL_Vec2> &out_uvs,
                std::vector<CL_Vec3> &out_normals);

  std::unique_ptr<ILogger> logger;
};
}
#endif  // COREENGINE_MODELLOADER