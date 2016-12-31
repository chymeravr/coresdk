#include <gtest/gtest.h>
#include <coreEngineTest/mocks/ModelMock.h>
#include <coreEngineTest/mocks/SceneMock.h>
#include <coreEngineTest/mocks/LoggerFactoryMock.h>

namespace cl{
	class ModelTest : public ::testing::Test{
	protected:
		void SetUp(){
			std::unique_ptr<ILoggerFactory> loggerFactory(new LoggerFactoryMock);
			model = std::unique_ptr<Model>(new ModelMock(modelId, loggerFactory.get()));
		}
		void TearDown(){
		}
		std::string modelId = "model1";
		const std::string modelType = "model";
		std::unique_ptr<Model> model;
	};

	TEST_F(ModelTest, TestModelCreation){
		EXPECT_EQ(modelId, model->getUniqueIdentifier());
		EXPECT_EQ(modelType, model->getType());
		EXPECT_EQ(modelId, model->getSceneId());
	}
}