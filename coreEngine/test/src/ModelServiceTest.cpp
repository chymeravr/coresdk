#include <memory>
#include <gtest/gtest.h>
#include <coreEngine/factory/IModelFactoy.h>
#include <coreEngine/factory/defaultImpl/ModelFactory.h>
#include <coreEngine/util/ILoggerFactory.h>
#include <coreEngine/service/IModelService.h>
#include <coreEngine/service/defaultImpl/ModelService.h>
#include <coreEngine/service/IObjectService.h>
#include <coreEngine/service/defaultImpl/ObjectService.h>
#include <coreEngineTest/mocks/ModelRendererMock.h>
#include <coreEngineTest/mocks/LoggerFactoryMock.h>


namespace cl{
    class ModelServiceTest : public ::testing::Test{
    protected:
        std::unique_ptr<IModelFactory> modelFactoryPtr;
        std::unique_ptr<ILoggerFactory> loggerFactoryPtr;
        std::unique_ptr<IModelRenderer> modelRendererPtr;
        std::string tag = "tag";
        std::unique_ptr<IModelService> modelServicePtr;
        virtual void SetUp(){
            modelFactoryPtr = std::unique_ptr<IModelFactory>(new ModelFactory());
            modelRendererPtr = std::unique_ptr<IModelRenderer>(new ModelRendererMock());
            loggerFactoryPtr = std::unique_ptr<ILoggerFactory>(new LoggerFactoryMock());
            modelServicePtr = std::unique_ptr<IModelService>(new ModelService(loggerFactoryPtr.get(), std::unique_ptr<IObjectService>(new ObjectService(loggerFactoryPtr.get()))));
        }
        virtual void TearDown(){

        }
    };

    /*TEST_F(ModelServiceTest, TestConvertQuadIndicesToTriangleIndices){
        std::unique_ptr<Object> modelUPtr = modelFactoryPtr->create(tag, modelRendererPtr.get());
        Model *modelPtr = (Model*)modelUPtr.get();
        std::vector<CL_GLuint> &indices = modelPtr->getIndices();
        EXPECT_EQ(0, indices.size());
        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(3);
        indices.push_back(4);
        indices.push_back(5);
        indices.push_back(6);
        indices.push_back(7);
        indices.push_back(8);
        modelServicePtr->convertQuadIndicesToTriangleIndices(modelPtr);
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
    }*/

    TEST_F(ModelServiceTest, TestInvertNormal){
        std::unique_ptr<Object> modelUPtr = modelFactoryPtr->create(tag, std::move(modelRendererPtr));
        Model *modelPtr = (Model*)modelUPtr.get();
        std::vector<CL_GLuint> &indices = modelPtr->getIndices();
        EXPECT_EQ(0, indices.size());
        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(3);
        indices.push_back(4);
        indices.push_back(5);
        indices.push_back(6);
        EXPECT_EQ(6, indices.size());
        modelServicePtr->invertNormal(modelPtr);
        std::vector<CL_GLuint> &newIndices = modelPtr->getIndices();
        EXPECT_EQ(6, newIndices.size());
        EXPECT_EQ(1, newIndices[0]);
        EXPECT_EQ(3, newIndices[1]);
        EXPECT_EQ(2, newIndices[2]);
        EXPECT_EQ(4, newIndices[3]);
        EXPECT_EQ(6, newIndices[4]);
        EXPECT_EQ(5, newIndices[5]);
    }
    /*
        How to test that??
        Currently testing only number of items in vectors.
    */
    TEST_F(ModelServiceTest, TestBuildInwardCube){
        std::unique_ptr<Object> modelUPtr = modelFactoryPtr->create(tag, std::move(modelRendererPtr));
        Model *modelPtr = (Model*)modelUPtr.get();
        modelServicePtr->buildInwardCube(modelPtr);
        std::vector<CL_GLuint> &indices = modelPtr->getIndices();
        std::vector <CL_Vec3> &vertices = modelPtr->getVertices();
        std::vector <CL_Vec2> &uvs = modelPtr->getUvs();

        EXPECT_EQ(36, indices.size());
        EXPECT_EQ(24, vertices.size());
        EXPECT_EQ(24, uvs.size());
    }

    /*
    How to test that??
    Currently testing only number of items in vectors.
    */
    TEST_F(ModelServiceTest, TestBuildOutwardCube){
        std::unique_ptr<Object> modelUPtr = modelFactoryPtr->create(tag, std::move(modelRendererPtr));
        Model *modelPtr = (Model*)modelUPtr.get();
        modelServicePtr->buildOutwardCube(modelPtr);
        std::vector<CL_GLuint> &indices = modelPtr->getIndices();
        std::vector <CL_Vec3> &vertices = modelPtr->getVertices();
        std::vector <CL_Vec2> &uvs = modelPtr->getUvs();

        EXPECT_EQ(36, indices.size());
        EXPECT_EQ(24, vertices.size());
        EXPECT_EQ(24, uvs.size());
    }
}