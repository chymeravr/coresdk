#include <coreEngine/conf/MathType.h>
#include <coreEngine/modelBuilder/modelLoader.h>
#include <coreEngineTest/mocks/LoggerFactoryMock.h>
#include <coreEngineTest/mocks/ModelMock.h>
#include <gtest/gtest.h>

/*
  Use Cube Model defined below to test ModelLoader
  http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
*/

namespace cl {
class ModelLoaderTest : public ::testing::Test {
 protected:
  void SetUp() {
    std::unique_ptr<ILoggerFactory> loggerFactory(new LoggerFactoryMock);
    model = std::unique_ptr<Model>(new ModelMock(modelId, loggerFactory.get()));
    modelLoader =
        std::unique_ptr<ModelLoader>(new ModelLoader(loggerFactory.get()));
    modelLoader->load_obj("cube.obj", model.get());

    vertices = model->getVertices();
    uvs = model->getUvs();
    normals = model->getNormals();
    elements = model->getIndices();
  }
  void TearDown() {}
  std::string modelId = "model1";
  const std::string modelType = "model";

  std::unique_ptr<Model> model;
  std::unique_ptr<ModelLoader> modelLoader;

  std::vector<CL_Vec3> vertices;
  std::vector<CL_Vec2> uvs;
  std::vector<CL_Vec3> normals;     // Won't be used at the moment.
  std::vector<CL_GLuint> elements;  // Won't be used at the moment.
};
TEST_F(ModelLoaderTest, TestModelLoading) {
  EXPECT_EQ(vertices.size(), 28);
  EXPECT_EQ(uvs.size(), 28);
  EXPECT_EQ(normals.size(), 28);
  EXPECT_EQ(elements.size(), 36);
}
}
