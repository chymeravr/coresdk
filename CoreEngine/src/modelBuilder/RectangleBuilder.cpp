#include <coreEngine/conf/Types.h>
#include <coreEngine/modelBuilder/RectangleBuilder.h>

// todo ~ what is the texture mappings need to change as per the client
// applications requirements
namespace cl {
RectangleBuilder::RectangleBuilder(ILoggerFactory *loggerFactory) {
  this->logger = loggerFactory->createLogger("CoreEngine::RectangleBuilder: ");
}

void RectangleBuilder::buildRectangle(Model *model, float width, float height) {
  std::vector<CL_Vec3> &vertices = model->getVertices();
  vertices.push_back(CL_Vec3(-width / 2, -height / 2, 0.0));  // left bottom
  vertices.push_back(CL_Vec3(width / 2, -height / 2, 0.0));   // right bottom
  vertices.push_back(CL_Vec3(width / 2, height / 2, 0.0));    // right top
  vertices.push_back(CL_Vec3(-width / 2, height / 2, 0.0));   // left top
  std::vector<CL_GLuint> &indices = model->getIndices();
  indices.push_back(0);
  indices.push_back(1);
  indices.push_back(3);
  indices.push_back(1);
  indices.push_back(2);
  indices.push_back(3);

  std::vector<CL_Vec2> &uvs = model->getUvs();
  CL_Vec2 uvTopLeft, uvTopRight, uvBottomLeft, uvBottomRight;
  uvTopLeft.x = 0.0f;
  uvTopLeft.y = 1.0f;

  uvTopRight.x = 1.0f;
  uvTopRight.y = 1.0f;

  uvBottomRight.x = 1.0f;
  uvBottomRight.y = 0.5f;

  uvBottomLeft.x = 0.0f;
  uvBottomLeft.y = 0.5f;

  uvs.push_back(uvBottomLeft);
  uvs.push_back(uvBottomRight);
  uvs.push_back(uvTopRight);
  uvs.push_back(uvTopLeft);
}
}