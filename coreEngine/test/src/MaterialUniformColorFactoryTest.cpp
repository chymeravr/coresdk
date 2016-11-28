#include <gtest/gtest.h>
#include <coreEngine/factory/defaultImpl/MaterialUniformColorFactory.h>
#include <coreEngine/service/defaultImpl/ObjectService.h>
#include <coreEngine/factory/defaultImpl/ShaderFactory.h>

#include <coreEngineTest/mocks/LoggerFactoryMock.h>
#include <coreEngineTest/mocks/ShaderRendererMock.h>
#include <coreEngineTest/mocks/MaterialRendererMock.h>

using namespace cl;

TEST(MaterialUniformColorFactoryTest, TestMaterialCreation){
    std::string materialTag = "materialTag";
    std::unique_ptr<ILoggerFactory> loggerFactoryPtr(new LoggerFactoryMock);
    std::unique_ptr<IObjectService> objectServicePtr(new ObjectService(loggerFactoryPtr.get()));
    std::unique_ptr<IMaterialFactory> materialFactoryPtr(new MaterialUniformColorFactory(std::move(objectServicePtr)));

    std::string shaderTag = "shaderTag";
    std::unique_ptr<IShaderFactory> shaderFactoryPtr(new ShaderFactory());
    std::unique_ptr<IShaderRenderer> shaderRendererPtr(new ShaderRendererMock);
    std::unique_ptr<Object> shaderUPtr = shaderFactoryPtr->create(shaderTag, std::move(shaderRendererPtr));
    Shader *shaderPtr = (Shader*)shaderUPtr.get();

    std::unique_ptr<IMaterialRenderer> materialRenderer(new MaterialRendererMock);
    std::unique_ptr<Object> materialUPtr = materialFactoryPtr->create(materialTag, shaderPtr, std::move(materialRenderer));
    EXPECT_NE(nullptr, materialUPtr);
    EXPECT_EQ(materialTag, materialUPtr->getTag());
    EXPECT_EQ("material", materialUPtr->getObjectType());

    MaterialUniformColor *materialPtr = (MaterialUniformColor*)materialUPtr.get();
    materialPtr->setColor(CL_Vec3(1.0, 1.0, 1.0));
    EXPECT_THAT(CL_Vec3(1.0, 1.0, 1.0), materialPtr->getColor());
}