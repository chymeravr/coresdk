#include <gtest/gtest.h>
#include <coreEngineTest/mocks/ShaderMock.h>
#include <coreEngineTest/mocks/SceneMock.h>
#include <coreEngineTest/mocks/LoggerFactoryMock.h>

namespace cl{
	class ShaderTest : public ::testing::Test{
	protected:
		void SetUp(){
			std::unique_ptr<ILoggerFactory> loggerFactory(new LoggerFactoryMock);
			scene = std::unique_ptr<Scene>(new SceneMock(loggerFactory.get(), sceneId));
			shader = std::unique_ptr<Shader>(new ShaderMock(shaderId, loggerFactory.get(), scene.get()));
		}
		void TearDown(){
		}
		std::string sceneId = "scene1";
		std::string shaderId = "shader1";
		const std::string shaderType = "shader";
		const std::string sceneType = "scene";
		std::unique_ptr<Scene> scene;
		std::unique_ptr<Shader> shader;
	};

	TEST_F(ShaderTest, TestShaderCreation){
		EXPECT_EQ(shaderId, shader->getUniqueIdentifier());
		EXPECT_EQ(shaderType, shader->getType());
		EXPECT_EQ(shaderId, shader->getSceneId());
		std::vector<Relation*> shaders = scene->getRelations(shaderType);
		EXPECT_EQ(1, shaders.size());
		EXPECT_EQ(shaderId, shaders[0]->getUniqueIdentifier());
		std::vector<Relation*> scenes = shader->getRelations(sceneType);
		EXPECT_EQ(1, scenes.size());
		EXPECT_EQ(sceneId, scenes[0]->getUniqueIdentifier());
	}
}