#include <coreEngine/conf/MathType.h>
#include <coreEngine/modelBuilder/modelLoader.h>
#include <coreEngineTest/mocks/ModelMock.h>
#include <coreEngineTest/mocks/LoggerFactoryMock.h>
#include <gtest/gtest.h>

namespace cl {
class ModelLoaderTest : public ::testing::Test {
 protected:
  void SetUp() {
	  std::unique_ptr<ILoggerFactory> loggerFactory(new LoggerFactoryMock);
	  model = std::unique_ptr<Model>(new ModelMock(modelId, loggerFactory.get()));
	  ModelLoader::load_obj("cube.obj", model.get());

	  vertices = model->getVertices();
	  uvs = model->getUvs();
	  normals = model->getNormals();
	  elements = model->getIndices();

  }
  void TearDown() {}
  std::string modelId = "model1";
  const std::string modelType = "model";

  std::unique_ptr<Model> model;

  std::vector<CL_Vec3> vertices;
  std::vector<CL_Vec2> uvs;
  std::vector<CL_Vec3> normals;     // Won't be used at the moment.
  std::vector<CL_GLuint> elements;  // Won't be used at the moment.
};
TEST_F(ModelLoaderTest, TestModelLoading) {
  EXPECT_EQ(vertices.size(), 8);
  EXPECT_EQ(uvs.size(), 0);
  EXPECT_EQ(normals.size(), 6);
  EXPECT_EQ(elements.size(), 36);
  //   EXPECT_EQ(materialId, material->getUniqueIdentifier());
  //   EXPECT_EQ(materialType, material->getType());
  //   EXPECT_EQ(materialId, material->getSceneId());

  //   std::vector<Relation*> materials = shader->getRelations(materialType);
  //   EXPECT_EQ(1, materials.size());
  //   EXPECT_EQ(materialId, materials[0]->getUniqueIdentifier());
  //   std::vector<Relation*> shaders = material->getRelations(shaderType);
  //   EXPECT_EQ(1, shaders.size());
  //   EXPECT_EQ(shaderId, shaders[0]->getUniqueIdentifier());
}
}
