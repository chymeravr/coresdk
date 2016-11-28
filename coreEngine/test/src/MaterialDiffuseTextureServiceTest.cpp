#include <gtest/gtest.h>
#include <coreEngine/factory/defaultImpl/MaterialDiffuseTextureFactory.h>
#include <coreEngine/model/MaterialDiffuseTexture.h>
#include <coreEngine/service/defaultImpl/ObjectService.h>
#include <coreEngine/factory/defaultImpl/ShaderFactory.h>
#include <coreEngine/service/defaultImpl/MaterialDiffuseTextureService.h>
#include <coreEngine/factory/defaultImpl/TextureFactory.h>
#include <coreEngine/model/Texture.h>
#include <coreEngineTest/mocks/LoggerFactoryMock.h>
#include <coreEngineTest/mocks/ShaderRendererMock.h>
#include <coreEngineTest/mocks/MaterialRendererMock.h>
#include <coreEngineTest/mocks/TextureRendererMock.h>

namespace cl{
    class MaterialDiffuseTextureServiceTest : public ::testing::Test{
    protected:
        std::unique_ptr<Object> materialUPtr1;
        std::unique_ptr<Object> materialUPtr2;
        std::unique_ptr<ILoggerFactory> loggerFactoryPtr;
        std::string materialTag1 = "materialTag1";
        std::string materialTag2 = "materialTag2";
        virtual void SetUp(){
            loggerFactoryPtr = std::unique_ptr<ILoggerFactory>(new LoggerFactoryMock);
            std::unique_ptr<IObjectService> objectServicePtr(new ObjectService(loggerFactoryPtr.get()));
            std::unique_ptr<IMaterialFactory> materialFactoryPtr(new MaterialDiffuseTextureFactory(std::move(objectServicePtr)));

            std::string shaderTag = "shaderTag";
            std::unique_ptr<IShaderFactory> shaderFactoryPtr(new ShaderFactory());
            std::unique_ptr<IShaderRenderer> shaderRendererPtr(new ShaderRendererMock);
            std::unique_ptr<Object> shaderUPtr = shaderFactoryPtr->create(shaderTag, std::move(shaderRendererPtr));
            Shader *shaderPtr = (Shader*)shaderUPtr.get();

            std::unique_ptr<IMaterialRenderer> materialRenderer(new MaterialRendererMock);
            materialUPtr1 = materialFactoryPtr->create(materialTag1, shaderPtr, std::move(materialRenderer));
            materialUPtr2 = materialFactoryPtr->create(materialTag2, shaderPtr, std::move(materialRenderer));
        }
        virtual void TearDown(){

        }
    };

    TEST_F(MaterialDiffuseTextureServiceTest, TestAddRemoveAndGetDiffuseTexture){
        std::unique_ptr<IMaterialDiffuseTextureService> diffuseMaterialService(new MaterialDiffuseTextureService(loggerFactoryPtr.get()));
        std::string textureTag1 = "textureTag1";
        std::string textureTag2 = "textureTag2";
        std::unique_ptr<ITextureFactory> textureFactoryPtr(new TextureFactory);
        std::unique_ptr<ITextureRenderer> textureRendererPtr(new TextureRendererMock);
        std::unique_ptr<Object> textureObjectUPtr1 = textureFactoryPtr->create(textureTag1, std::move(textureRendererPtr));
        Texture *texturePtr1 = (Texture*)textureObjectUPtr1.get();
        std::unique_ptr<Object> textureObjectUPtr2 = textureFactoryPtr->create(textureTag2, std::move(textureRendererPtr));
        Texture *texturePtr2 = (Texture*)textureObjectUPtr2.get();

        MaterialDiffuseTexture *materialPtr1 = (MaterialDiffuseTexture*)materialUPtr1.get();
        MaterialDiffuseTexture *materialPtr2 = (MaterialDiffuseTexture*)materialUPtr2.get();
        Texture *texturePtr = diffuseMaterialService->getDiffuseTexture(materialPtr1);
        EXPECT_EQ(nullptr, texturePtr);

        diffuseMaterialService->addDiffuseTexture(materialPtr1, texturePtr1);
        texturePtr = diffuseMaterialService->getDiffuseTexture(materialPtr1);
        EXPECT_NE(nullptr, texturePtr);
        EXPECT_EQ(textureTag1, texturePtr->getTag());

        diffuseMaterialService->addDiffuseTexture(materialPtr1, texturePtr2);
        texturePtr = diffuseMaterialService->getDiffuseTexture(materialPtr1);
        EXPECT_NE(nullptr, texturePtr);
        EXPECT_EQ(textureTag2, texturePtr->getTag());

        diffuseMaterialService->removeDiffuseTexture(materialPtr1);
        texturePtr = diffuseMaterialService->getDiffuseTexture(materialPtr1);
        EXPECT_EQ(nullptr, texturePtr);
    }
}