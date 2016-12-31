#include <gtest/gtest.h>
#include <coreEngine/modifier/ModelModifier.h>
#include <coreEngineTest/mocks/ModelMock.h>
#include <coreEngineTest/mocks/LoggerFactoryMock.h>
#include <coreEngine/modelBuilder/UVSphereBuilder.h>

namespace cl{
	class SphereBuilderTest : public ::testing::Test{
	protected:
		void SetUp(){
			std::unique_ptr<ILoggerFactory> loggerFactory(new LoggerFactoryMock);
			model = std::unique_ptr<Model>(new ModelMock(modelId, loggerFactory.get()));
			modelModifier = std::unique_ptr<ModelModifier>(new ModelModifier);
		}
		void TearDown(){
		}

		void sphereTestHelper(unsigned int nSubdivisionTimes){
			UVSphereBuilder sphereBuilder(modelModifier.get());
			double distanceFromCenter = 1.0;

			sphereBuilder.buildUnitSphere(model.get(), nSubdivisionTimes);
			std::vector<CL_GLuint> &indices = model->getIndices();
			std::vector<CL_Vec3> &vertices = model->getVertices();

			//Check distance from center
			for (unsigned int i = 0; i < vertices.size(); i++){
				float x = vertices[i].x;
				float y = vertices[i].y;
				float z = vertices[i].z;
				double distance = sqrt(x*x + y*y + z*z);
				EXPECT_NEAR(distanceFromCenter, distance, precisionError);
				//std::cout << x << "\t" << y << "\t" << z << std::endl;
			}
			/**
			* The following code is not working. So commented out.
			*
			*/
			//Test if normal of triangles faces center
			/*float polePointThres = 0.01; //don't consider near pole points
			for (unsigned int i = 0; i < indices.size(); i+=3){
			CL_Vec3 v1 = vertices[indices[i]];
			CL_Vec3 v2 = vertices[indices[i + 1]];
			CL_Vec3 v3 = vertices[indices[i + 2]];

			if (abs(v1.x) < polePointThres || abs(v1.z) < polePointThres){
			continue;
			}
			if (abs(v2.x) < polePointThres || abs(v2.z) < polePointThres){
			continue;
			}if (abs(v3.x) < polePointThres || abs(v3.z) < polePointThres){
			continue;
			}
			CL_Vec3 t = getLineParameterForTestingNormalHelper(vertices[indices[i]], vertices[indices[i + 1]], vertices[indices[i + 2]]);
			EXPECT_NEAR(t.x, t.y, precisionError);
			EXPECT_NEAR(t.y, t.z, precisionError);
			EXPECT_GE(t.x, 0.0);
			}*/
		}

		CL_Vec3 getLineParameterForTestingNormalHelper(CL_Vec3 v1, CL_Vec3 v2, CL_Vec3 v3){
			// m is middle point of the triangle
			CL_Vec3 m = (v1 + v2 + v3);
			m /= 3;

			// normal to the triangle
			CL_Vec3 normal = CL_Normalize(CL_CrossProduct(v2 - v1, v3 - v1));

			// Checking that line passing through m in normal direction passes through origin and has line parameter
			// positive (implying origin is in the direction of normal).
			float tx = (0.0f - m.x) / normal.x;
			float ty = (0.0f - m.y) / normal.y;
			float tz = (0.0f - m.z) / normal.z;
			return CL_Vec3(tx, ty, tz);
		}
		std::string modelId = "model1";
		const std::string modelType = "model";
		std::unique_ptr<Model> model;
		std::unique_ptr<ModelModifier> modelModifier;
		double precisionError = 0.01;
	};

	TEST_F(SphereBuilderTest, TestLineParameterHelperTestingFunction){
		CL_Vec3 v1(1.0f, 0.0f, 0.0f);
		CL_Vec3 v2(0.0f, 1.0f, 0.0f);
		CL_Vec3 v3(0.0f, 0.0f, 1.0f);
		CL_Vec3 t = getLineParameterForTestingNormalHelper(v3, v2, v1); //facing origin
		EXPECT_NEAR(t.x, t.y, precisionError);
		EXPECT_NEAR(t.y, t.z, precisionError);
		EXPECT_GE(t.x, 0.0);

		t = getLineParameterForTestingNormalHelper(v1, v2, v3); //facing away from origin
		EXPECT_NEAR(t.x, t.y, precisionError);
		EXPECT_NEAR(t.y, t.z, precisionError);
		EXPECT_LE(t.x, 0.0);
	}

	/*
	* It needs to be tested manually to see if actual sphere is there. Use https://plot.ly/create/ to check if points are on the sphere.
	*/
	TEST_F(SphereBuilderTest, TestBuildUnitSphereBaseSize){
		sphereTestHelper(0);
	}
	TEST_F(SphereBuilderTest, TestBuildUnitSphereDivisions1){
		sphereTestHelper(1);
	}
	TEST_F(SphereBuilderTest, TestBuildUnitSphereDivisions6){
		sphereTestHelper(6);
	}

	/*Diufficult to test this*/
	/*TEST_F(SphereBuilderTest, TestUVMap){
	}*/

	TEST_F(SphereBuilderTest, TestGenerateUVMapForAllVertices){
		UVSphereBuilder sphereBuilder(modelModifier.get());
		unsigned int nSubdivisionTimes = 5;
		sphereBuilder.buildUnitSphere(model.get(), nSubdivisionTimes);
		EXPECT_EQ(model->getVertices().size(), model->getUvs().size());
	}
}