#include <coreEngine/modelBuilder/ConcentricCircleBuilder.h>

namespace cl {
void ConcentricCircleBuilder::buildConcentricCircle(Model *model, float rInner,
                                                    float rOuter,
                                                    unsigned int nDivisions) {
  assert(model != nullptr);
  float stepTheta = 2 * CL_PI / nDivisions;
  float currentTheta = 0.0f;
  std::vector<CL_Vec3> &vertices = model->getVertices();
  std::vector<CL_GLuint> &indices = model->getIndices();

  CL_Vec3 previousInnerPoint =
      CL_Vec3(rInner * cosf(currentTheta), rInner * sinf(currentTheta), 0.0f);
  CL_Vec3 previousOuterPoint =
      CL_Vec3(rOuter * cosf(currentTheta), rOuter * sinf(currentTheta), 0.0f);
  vertices.push_back(previousInnerPoint);
  vertices.push_back(previousOuterPoint);
  for (unsigned int i = 1; i <= nDivisions; i++) {
    currentTheta += stepTheta;
    CL_Vec3 currentInnerPoint =
        CL_Vec3(rInner * cosf(currentTheta), rInner * sinf(currentTheta), 0.0f);
    CL_Vec3 currentOuterPoint =
        CL_Vec3(rOuter * cosf(currentTheta), rOuter * sinf(currentTheta), 0.0f);
    vertices.push_back(currentInnerPoint);
    vertices.push_back(currentOuterPoint);
    indices.push_back(2 * (i - 1));
    indices.push_back(2 * (i - 1) + 1);
    indices.push_back(2 * i);
    indices.push_back(2 * i);
    indices.push_back(2 * (i - 1) + 1);
    indices.push_back(2 * i + 1);
    previousInnerPoint = currentInnerPoint;
    previousOuterPoint = currentOuterPoint;
  }

  model->setBlending(false);
  model->setDepthTest(true);
}
}