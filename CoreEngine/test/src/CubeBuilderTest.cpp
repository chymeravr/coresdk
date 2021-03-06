#include <gtest/gtest.h>
#include <coreEngine/modifier/ModelModifier.h>
#include <coreEngineTest/mocks/ModelMock.h>
#include <coreEngineTest/mocks/LoggerFactoryMock.h>
#include <coreEngine/modelBuilder/CubeBuilder.h>

namespace cl{
	class CubeBuilderTest : public ::testing::Test{
	protected:
		void SetUp(){
			std::unique_ptr<ILoggerFactory> loggerFactory(new LoggerFactoryMock);
			model = std::unique_ptr<Model>(new ModelMock(modelId, loggerFactory.get()));
			modelModifier = std::unique_ptr<ModelModifier>(new ModelModifier);
		}
		void TearDown(){
		}
		std::string modelId = "model1";
		const std::string modelType = "model";
		std::unique_ptr<Model> model;
		std::unique_ptr<ModelModifier> modelModifier;
		double precisionError = 0.01;
	};

	/*
	* Limited Testing.
	*/
	TEST_F(CubeBuilderTest, TestBuildInwardCube){
		CubeBuilder cubeBuilder(modelModifier.get());
		cubeBuilder.buildInwardCube(model.get());
		std::vector<CL_GLuint> &indices = model->getIndices();
		std::vector <CL_Vec3> &vertices = model->getVertices();
		std::vector <CL_Vec2> &uvs = model->getUvs();

		EXPECT_EQ(36, indices.size());
		EXPECT_EQ(24, vertices.size());
		EXPECT_EQ(24, uvs.size());

		//Check distance from center
		double targetDistance = 1.732;
		for (int i = 0; i < vertices.size(); i++){
			float x = vertices[i].x;
			float y = vertices[i].y;
			float z = vertices[i].z;
			double distance = sqrt(x*x + y*y + z*z);
			EXPECT_NEAR(targetDistance, distance, precisionError);
		}
	}

	/*
	* Limited Testing. Testing number of vertices only. Can't test each vertex as it is difficult.
	*/
	TEST_F(CubeBuilderTest, TestBuildOutwardCube){
		CubeBuilder cubeBuilder(modelModifier.get());
		cubeBuilder.buildOutwardCube(model.get());
		std::vector<CL_GLuint> &indices = model->getIndices();
		std::vector <CL_Vec3> &vertices = model->getVertices();
		std::vector <CL_Vec2> &uvs = model->getUvs();

		EXPECT_EQ(36, indices.size());
		EXPECT_EQ(24, vertices.size());
		EXPECT_EQ(24, uvs.size());

		//Check distance from center
		double targetDistance = 1.732;
		for (int i = 0; i < vertices.size(); i++){
			float x = vertices[i].x;
			float y = vertices[i].y;
			float z = vertices[i].z;
			double distance = sqrt(x*x + y*y + z*z);
			EXPECT_NEAR(targetDistance, distance, precisionError);
		}
	}
}