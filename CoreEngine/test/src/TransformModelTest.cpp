#include <gtest/gtest.h>
#include <coreEngine/components/transform/TransformModel.h>
#include <coreEngineTest/mocks/ModelMock.h>
#include <coreEngineTest/mocks/LoggerFactoryMock.h>

namespace cl{
    class TransformModelTest : public ::testing::Test{
    protected:
        void SetUp(){
            std::unique_ptr<ILoggerFactory> loggerFactory(new LoggerFactoryMock);
            model = std::unique_ptr<Model>(new ModelMock(modelId, loggerFactory.get()));
            transformModel = std::unique_ptr<TransformModel>(new TransformModel(model.get(), loggerFactory.get()));
        }
        void TearDown(){

        }
        std::string modelId = "model1";
        const std::string modelType = "model";
        std::unique_ptr<Model> model;
        std::unique_ptr<TransformModel> transformModel;
        float precision = 0.01f;
    };

    TEST_F(TransformModelTest, TestGetComponentType){
        EXPECT_EQ("transform", transformModel->getComponentType());
    }

    TEST_F(TransformModelTest, TestDraw){
        transformModel->setPosition(CL_Vec3(3.0f, 0.0f, 0.0f));
        transformModel->setRotation(CL_Vec3(0.0f, 45.0f, 0.0f));
        transformModel->setScale(CL_Vec3(1.0f, 1.0f, 1.0f));
        transformModel->draw();
        CL_Mat44 modelMatrix = model->getModelMatrix();
        EXPECT_NEAR(0.71, modelMatrix[0][0], precision);
        EXPECT_NEAR(0.00, modelMatrix[0][1], precision);
        EXPECT_NEAR(-0.71, modelMatrix[0][2], precision);
        EXPECT_NEAR(0.00, modelMatrix[0][3], precision);
        EXPECT_NEAR(0.00, modelMatrix[1][0], precision);
        EXPECT_NEAR(1.00, modelMatrix[1][1], precision);
        EXPECT_NEAR(0.00, modelMatrix[1][2], precision);
        EXPECT_NEAR(0.00, modelMatrix[1][3], precision);
        EXPECT_NEAR(0.71, modelMatrix[2][0], precision);
        EXPECT_NEAR(0.00, modelMatrix[2][1], precision);
        EXPECT_NEAR(0.71, modelMatrix[2][2], precision);
        EXPECT_NEAR(0.00, modelMatrix[2][3], precision);
        EXPECT_NEAR(3.00, modelMatrix[3][0], precision);
        EXPECT_NEAR(0.00, modelMatrix[3][1], precision);
        EXPECT_NEAR(0.00, modelMatrix[3][2], precision);
        EXPECT_NEAR(1.00, modelMatrix[3][3], precision);
    }
}