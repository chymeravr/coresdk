#include <gtest/gtest.h>
#include <coreEngine/factory/SceneFactory.h>
#include <coreEngineTest/mocks/SceneRendererMock.h>

using namespace cl;

TEST(SceneFactoryTest, TestSceneCreation){
	std::unique_ptr<ISceneRenderer> sceneRendererPtr(new SceneRendererMock());
	SceneFactory sceneFactory;
	std::unique_ptr<Scene> scenePtr = sceneFactory.createScene(std::move(sceneRendererPtr));
	EXPECT_NE(nullptr, scenePtr);
}