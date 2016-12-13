#include <gtest/gtest.h>
#include <coreEngine/renderObjects/Scene.h>
#include <coreEngineTest/mocks/LoggerFactoryMock.h>
#include <coreEngineTest/mocks/SceneMock.h>
#include <coreEngineTest/mocks/ScenableMock.h>

namespace cl{
    class SceneTest : public ::testing::Test{
        protected:
            void SetUp(){
                loggerFactoryUptr = std::unique_ptr<ILoggerFactory>(new LoggerFactoryMock);
                scene = std::unique_ptr<Scene>(new SceneMock(loggerFactoryUptr.get()));
            }
            
            void TearDown(){

            }
            std::unique_ptr<ILoggerFactory> loggerFactoryUptr;
            std::unique_ptr<Scene> scene;
    };

    TEST_F(SceneTest, TestGetType){
        EXPECT_EQ("scene", scene->getType());
    }

    TEST_F(SceneTest, TestSceneIdNotExists){
        EXPECT_FALSE(scene->exists("randomSceneId"));
    }

    TEST_F(SceneTest, TestAddScenable){
        std::string sceneId1 = "sceneId1";
        std::unique_ptr<IScenable> scenable1(new ScenableMock(sceneId1));
        scene->addToScene(std::move(scenable1));
        EXPECT_TRUE(scene->exists(sceneId1));

        // Testing assertion for duplicate sceneId
        {
            std::unique_ptr<IScenable> scenable1(new ScenableMock(sceneId1));
            EXPECT_DEATH(scene->addToScene(std::move(scenable1)), "");
        }

        // Testing scene with multiple sceneIds
        std::string sceneId2 = "sceneId2";
        std::unique_ptr<IScenable> scenable2(new ScenableMock(sceneId2));
        scene->addToScene(std::move(scenable2));
        EXPECT_TRUE(scene->exists(sceneId1));
        EXPECT_TRUE(scene->exists(sceneId2));
    }

    TEST_F(SceneTest, TestGetFromScene){
        {
            std::string sceneId = "sceneId1";
            EXPECT_EQ(nullptr, scene->getFromScene(sceneId));
            std::unique_ptr<IScenable> scenable(new ScenableMock(sceneId));
            scene->addToScene(std::move(scenable));
            EXPECT_NE(nullptr, scene->getFromScene(sceneId));
            EXPECT_EQ(sceneId, scene->getFromScene(sceneId)->getSceneId());
        }
        {
            std::string sceneId = "sceneId2";
            EXPECT_EQ(nullptr, scene->getFromScene(sceneId));
            std::unique_ptr<IScenable> scenable(new ScenableMock(sceneId));
            scene->addToScene(std::move(scenable));
            EXPECT_NE(nullptr, scene->getFromScene(sceneId));
            EXPECT_EQ(sceneId, scene->getFromScene(sceneId)->getSceneId());
        }
    }

    TEST_F(SceneTest, TestRemoveFromScene){
        EXPECT_DEATH(scene->removeFromScene(NULL), "");

        std::string sceneId1 = "sceneId1";
        std::string sceneId2 = "sceneId2";
        std::unique_ptr<IScenable> scenable1(new ScenableMock(sceneId1));
        std::unique_ptr<IScenable> scenable2(new ScenableMock(sceneId2));
        EXPECT_DEATH(scene->removeFromScene(scenable1.get()), "");
        scene->addToScene(std::move(scenable1));
        scene->addToScene(std::move(scenable2));

        EXPECT_DEATH(scene->removeFromScene(scenable1.get()), "");
        EXPECT_TRUE(scene->exists(sceneId1));
        IScenable *returnedScenable1 = scene->getFromScene(sceneId1);
        scene->removeFromScene(returnedScenable1);
        EXPECT_FALSE(scene->exists(sceneId1));
        EXPECT_TRUE(scene->exists(sceneId2));

        IScenable *returnedScenable2 = scene->getFromScene(sceneId2);
        scene->removeFromScene(returnedScenable2);
        EXPECT_FALSE(scene->exists(sceneId2));
    }
}