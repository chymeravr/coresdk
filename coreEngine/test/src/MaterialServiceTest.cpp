#include <gtest/gtest.h>
#include <coreEngine/factory/IMaterialFactory.h>
#include <coreEngine/factory/defaultImpl/MaterialFactory.h>
#include <coreEngine/service/defaultImpl/ObjectService.h>
#include <coreEngine/factory/defaultImpl/ShaderFactory.h>
#include <coreEngine/service/defaultImpl/MaterialService.h>

#include <coreEngineTest/mocks/LoggerFactoryMock.h>
#include <coreEngineTest/mocks/MaterialRendererMock.h>
#include <coreEngineTest/mocks/ShaderRendererMock.h>

#include <coreEngine/factory/defaultImpl/ModelFactory.h>
#include <coreEngineTest/mocks/ModelRendererMock.h>

namespace cl{
    class MaterialServiceTest : public ::testing::Test{
    protected:
        std::string modelTag = "modelTag";
        std::string materialTag = "materialTag";
        std::string shaderTag = "shaderTag";
        std::unique_ptr<ILoggerFactory> loggerFactoryPtr;
        std::unique_ptr<Object> shaderPtr;
        std::unique_ptr<IMaterialFactory> materialFactoryPtr;
        std::unique_ptr<IModelFactory> modelFactoryPtr;
        std::unique_ptr<IModelRenderer> modelRendererPtr;
        std::unique_ptr<IMaterialRenderer> materialRendererPtr;
        virtual void SetUp(){
            loggerFactoryPtr = std::unique_ptr<ILoggerFactory>(new LoggerFactoryMock);
            std::unique_ptr<IObjectService> objectServicePtr(new ObjectService(loggerFactoryPtr.get()));
            materialFactoryPtr = std::unique_ptr<IMaterialFactory>(new MaterialFactory(std::move(objectServicePtr)));

            std::unique_ptr<IShaderFactory> shaderFactoryPtr(new ShaderFactory());
            std::unique_ptr<IShaderRenderer> shaderRendererPtr(new ShaderRendererMock());
            shaderPtr = shaderFactoryPtr->create(shaderTag, std::move(shaderRendererPtr));

            materialRendererPtr = std::unique_ptr<IMaterialRenderer>(new MaterialRendererMock);

            std::string tag = "tag";
            modelFactoryPtr = std::unique_ptr<IModelFactory>(new ModelFactory());
            modelRendererPtr = std::unique_ptr<IModelRenderer>(new ModelRendererMock());
        }
        virtual void TearDown(){
            
        }
    };

    TEST_F(MaterialServiceTest, TestAddMaterialToModelAndGetModels){
        materialRendererPtr = std::unique_ptr<IMaterialRenderer>(new MaterialRendererMock);
        std::unique_ptr<Object> materialUPtr = materialFactoryPtr->create(materialTag, (Shader*)shaderPtr.get(), std::move(materialRendererPtr));
        modelRendererPtr = std::unique_ptr<IModelRenderer>(new ModelRendererMock());
        std::unique_ptr<Object> modelUPtr = modelFactoryPtr->create(modelTag, std::move(modelRendererPtr));
        Material *materialPtr = (Material*)materialUPtr.get();
        Model *modelPtr = (Model*)modelUPtr.get();
        
        std::unique_ptr<IObjectService> objectServicePtr(new ObjectService(loggerFactoryPtr.get()));
        std::unique_ptr<IMaterialService> materialServicePtr(new MaterialService(loggerFactoryPtr.get(), std::move(objectServicePtr)));
        EXPECT_NE(nullptr, materialServicePtr);
        
        std::vector<Model*> models = materialServicePtr->getModelsOfMaterial(materialPtr);
        EXPECT_EQ(0, models.size());
        
        //Test addition of material
        materialServicePtr->addMaterialToModel(materialPtr, modelPtr);
        models = materialServicePtr->getModelsOfMaterial(materialPtr);
        EXPECT_EQ(1, models.size());
        EXPECT_EQ(modelTag, models[0]->getTag());

        //Test for duplication
        materialServicePtr->addMaterialToModel(materialPtr, modelPtr);
        models = materialServicePtr->getModelsOfMaterial(materialPtr);
        EXPECT_EQ(1, models.size());
        EXPECT_EQ(modelTag, models[0]->getTag());
    }

    TEST_F(MaterialServiceTest, TestRemoveMaterialFromModel){
        materialRendererPtr = std::unique_ptr<IMaterialRenderer>(new MaterialRendererMock);
        std::unique_ptr<Object> materialUPtr = materialFactoryPtr->create(materialTag, (Shader*)shaderPtr.get(), std::move(materialRendererPtr));
        modelRendererPtr = std::unique_ptr<IModelRenderer>(new ModelRendererMock());
        std::unique_ptr<Object> modelUPtr = modelFactoryPtr->create(modelTag, std::move(modelRendererPtr));
        Material *materialPtr = (Material*)materialUPtr.get();
        Model *modelPtr = (Model*)modelUPtr.get();

        std::unique_ptr<IObjectService> objectServicePtr(new ObjectService(loggerFactoryPtr.get()));
        std::unique_ptr<IMaterialService> materialServicePtr(new MaterialService(loggerFactoryPtr.get(), std::move(objectServicePtr)));
        materialServicePtr->addMaterialToModel(materialPtr, modelPtr);
        std::vector<Model*> models = materialServicePtr->getModelsOfMaterial(materialPtr);
        EXPECT_EQ(1, models.size());

        //Test Deletion
        materialServicePtr->removeMaterialFromModel(materialPtr, modelPtr);
        models = materialServicePtr->getModelsOfMaterial(materialPtr);
        EXPECT_EQ(0, models.size());

        //Test double deletion
        materialServicePtr->removeMaterialFromModel(materialPtr, modelPtr);
        models = materialServicePtr->getModelsOfMaterial(materialPtr);
        EXPECT_EQ(0, models.size());
    }

    TEST_F(MaterialServiceTest, TestGetShader){
        materialRendererPtr = std::unique_ptr<IMaterialRenderer>(new MaterialRendererMock);
        std::unique_ptr<Object> materialUPtr = materialFactoryPtr->create(materialTag, (Shader*)shaderPtr.get(), std::move(materialRendererPtr));
        Material *materialPtr = (Material*)materialUPtr.get();

        std::unique_ptr<IObjectService> objectServicePtr(new ObjectService(loggerFactoryPtr.get()));
        std::unique_ptr<IMaterialService> materialServicePtr(new MaterialService(loggerFactoryPtr.get(), std::move(objectServicePtr)));

        Shader *shader = materialServicePtr->getShader(materialPtr);
        EXPECT_NE(nullptr, shader);
    }
}