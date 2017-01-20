#include <gtest/gtest.h>
#include <coreEngine/modifier/ModelModifier.h>
#include <coreEngineTest/mocks/ModelMock.h>
#include <coreEngineTest/mocks/LoggerFactoryMock.h>

namespace cl{
	class ModelModifierTest : public ::testing::Test{
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

	TEST_F(ModelModifierTest, TestQuadToTriangleConversion){
		ModelModifier modelModifier;
		std::vector<CL_GLuint> &indices = model->getIndices();
		EXPECT_EQ(0, indices.size());
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(3);
		indices.push_back(4);
		indices.push_back(5);
		indices.push_back(6);
		indices.push_back(7);
		indices.push_back(8);
		modelModifier.convertQuadIndicesToTriangleIndices(model.get());
		EXPECT_EQ(12, indices.size());
		EXPECT_EQ(1, indices[0]);
		EXPECT_EQ(2, indices[1]);
		EXPECT_EQ(3, indices[2]);
		EXPECT_EQ(1, indices[3]);
		EXPECT_EQ(3, indices[4]);
		EXPECT_EQ(4, indices[5]);

		EXPECT_EQ(5, indices[6]);
		EXPECT_EQ(6, indices[7]);
		EXPECT_EQ(7, indices[8]);
		EXPECT_EQ(5, indices[9]);
		EXPECT_EQ(7, indices[10]);
		EXPECT_EQ(8, indices[11]);
	}

	TEST_F(ModelModifierTest, TestInvertNormal){
		ModelModifier modelModifier;
		std::vector<CL_GLuint> &indices = model->getIndices();
		EXPECT_EQ(0, indices.size());
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(3);
		indices.push_back(4);
		indices.push_back(5);
		indices.push_back(6);
		EXPECT_EQ(6, indices.size());
		modelModifier.invertNormal(model.get());
		std::vector<CL_GLuint> &newIndices = model->getIndices();
		EXPECT_EQ(6, newIndices.size());
		EXPECT_EQ(1, newIndices[0]);
		EXPECT_EQ(3, newIndices[1]);
		EXPECT_EQ(2, newIndices[2]);
		EXPECT_EQ(4, newIndices[3]);
		EXPECT_EQ(6, newIndices[4]);
		EXPECT_EQ(5, newIndices[5]);
	}
}