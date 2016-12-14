#include <gtest/gtest.h>
#include <coreEngineTest/mocks/MaterialMock.h>
#include <coreEngineTest/mocks/SceneMock.h>
#include <coreEngineTest/mocks/LoggerFactoryMock.h>
#include <coreEngineTest/mocks/ShaderMock.h>

namespace cl{
    class MaterialTest : public ::testing::Test{
    protected:
        void SetUp(){
            std::unique_ptr<ILoggerFactory> loggerFactory(new LoggerFactoryMock);
            scene = std::unique_ptr<Scene>(new SceneMock(loggerFactory.get(), sceneId));
            shader = std::unique_ptr<Shader>(new ShaderMock(shaderId, loggerFactory.get(), scene.get()));
            material = std::unique_ptr<Material>(new MaterialMock(materialId, shader.get(), loggerFactory.get()));
        }
        void TearDown(){

        }
        std::string sceneId = "scene1";
        std::string shaderId = "shader1";
        const std::string shaderType = "shader";
        const std::string sceneType = "scene";
        std::unique_ptr<Scene> scene;
        std::unique_ptr<Shader> shader;
        std::string materialId = "material1";
        const std::string materialType = "material";
        std::unique_ptr<Material> material;
    };

    TEST_F(MaterialTest, TestMaterialCreation){
        EXPECT_EQ(materialId, material->getUniqueIdentifier());
        EXPECT_EQ(materialType, material->getType());
        EXPECT_EQ(materialId, material->getSceneId());

        std::vector<Relation*> materials = shader->getRelations(materialType);
        EXPECT_EQ(1, materials.size());
        EXPECT_EQ(materialId, materials[0]->getUniqueIdentifier());
        std::vector<Relation*> shaders = material->getRelations(shaderType);
        EXPECT_EQ(1, shaders.size());
        EXPECT_EQ(shaderId, shaders[0]->getUniqueIdentifier());
    }
}