#include <gtest/gtest.h>
#include <coreEngine/factory/IMaterialFactory.h>
#include <coreEngine/factory/defaultImpl/MaterialFactory.h>
#include <coreEngine/service/defaultImpl/ObjectService.h>
#include <coreEngine/factory/defaultImpl/ShaderFactory.h>

#include <coreEngineTest/mocks/LoggerFactoryMock.h>
#include <coreEngineTest/mocks/MaterialRendererMock.h>
#include <coreEngineTest/mocks/ShaderRendererMock.h>

using namespace cl;

TEST(MaterialFactoryTest, TestMaterialCreation){
    std::string materialTag = "materialTag";
    std::unique_ptr<ILoggerFactory> loggerFactoryPtr(new LoggerFactoryMock);
    std::unique_ptr<IObjectService> objectServicePtr(new ObjectService(loggerFactoryPtr.get()));
    std::unique_ptr<IMaterialFactory> materialFactoryPtr(new MaterialFactory(std::move(objectServicePtr)));

    std::string shaderTag = "shaderTag";
    std::unique_ptr<IShaderFactory> shaderFactoryPtr(new ShaderFactory());
    std::unique_ptr<IShaderRenderer> shaderRendererPtr(new ShaderRendererMock());
    std::unique_ptr<Object> shaderPtr = shaderFactoryPtr->create(shaderTag, std::move(shaderRendererPtr));

    std::unique_ptr<IMaterialRenderer> materialRendererPtr(new MaterialRendererMock);
    std::unique_ptr<Object> objectPtr = materialFactoryPtr->create(materialTag, (Shader*)shaderPtr.get(), std::move(materialRendererPtr));

    EXPECT_NE(nullptr, objectPtr);
    EXPECT_EQ(materialTag, objectPtr->getTag());
    EXPECT_EQ("material", objectPtr->getObjectType());

    std::unordered_map<std::string, std::vector<Object *>> &attachedObjectsByType = objectPtr->getAttachedObjectsByObjectType();
    EXPECT_EQ(1, attachedObjectsByType["shader"].size());
    EXPECT_EQ(shaderTag, attachedObjectsByType["shader"][0]->getTag());
}