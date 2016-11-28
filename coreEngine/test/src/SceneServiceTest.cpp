#include <memory>
#include <gtest/gtest.h>
#include <coreEngine/model/ISceneRenderer.h>
#include <coreEngine/factory/SceneFactory.h>
#include <coreEngine/factory/IObjectFactory.h>
#include <coreEngine/factory/defaultImpl/ObjectFactory.h>
#include <coreEngine/service/ISceneService.h>
#include <coreEngine/service/defaultImpl/SceneService.h>
#include <coreEngineTest/mocks/SceneRendererMock.h>
#include <coreEngineTest/mocks/LoggerMock.h>
#include <coreEngineTest/mocks/LoggerFactoryMock.h>

namespace cl{

    class SceneServiceTest : public ::testing::Test{
    protected:
        std::unique_ptr<Scene> scenePtr;
        std::unique_ptr<ILoggerFactory> loggerFactoryPtr;
        std::unique_ptr<IObjectFactory> objectFactoryPtr;
        virtual void SetUp(){
            std::unique_ptr<ISceneRenderer> sceneRendererPtr(new SceneRendererMock());
            SceneFactory sceneFactory;
            scenePtr = sceneFactory.createScene(std::move(sceneRendererPtr));
            loggerFactoryPtr = std::unique_ptr<ILoggerFactory>(new LoggerFactoryMock());
            objectFactoryPtr = std::unique_ptr<IObjectFactory>(new ObjectFactory());
        }
        virtual void TearDown(){

        }
    };

    /**
    * It tests only basic creation. It doesn't test for different edge cases.
    */
    TEST_F(SceneServiceTest, TestCreateObject){
        std::string tag1 = "tag1";
        std::unique_ptr<ISceneService> sceneServicePtr(new SceneService(loggerFactoryPtr.get()));

        Object *object = sceneServicePtr->createObject(scenePtr.get(), objectFactoryPtr.get(), tag1);
        EXPECT_NE(nullptr, object);
        EXPECT_EQ(tag1, object->getTag());
    }

    TEST_F(SceneServiceTest, TestIsTagAvailable){
        std::string tag1 = "tag1";
        std::unique_ptr<ISceneService> sceneServicePtr(new SceneService(loggerFactoryPtr.get()));
        EXPECT_TRUE(sceneServicePtr->isTagAvailable(scenePtr.get(), tag1));
        Object *object = sceneServicePtr->createObject(scenePtr.get(), objectFactoryPtr.get(), tag1);
        EXPECT_FALSE(sceneServicePtr->isTagAvailable(scenePtr.get(), tag1));
    }

    TEST_F(SceneServiceTest, TestGetObjectByTag){
        std::string tag1 = "tag1";
        std::unique_ptr<ISceneService> sceneServicePtr(new SceneService(loggerFactoryPtr.get()));
        Object *object = sceneServicePtr->getObjectByTag(scenePtr.get(), tag1);
        EXPECT_EQ(nullptr, object);
        sceneServicePtr->createObject(scenePtr.get(), objectFactoryPtr.get(), tag1);
        object = sceneServicePtr->getObjectByTag(scenePtr.get(), tag1);
        EXPECT_NE(nullptr, object);
        EXPECT_EQ(tag1, object->getTag());
        object = sceneServicePtr->getObjectByTag(scenePtr.get(), "tag2");
        EXPECT_EQ(nullptr, object);
    }

    TEST_F(SceneServiceTest, TestGetObjectsByObjectType){
        std::string tag1 = "tag1";
        std::unique_ptr<ISceneService> sceneServicePtr(new SceneService(loggerFactoryPtr.get()));
        std::vector<Object*> objects = sceneServicePtr->getObjectsByObjectType(scenePtr.get(), "Object");
        EXPECT_EQ(0, objects.size());
        sceneServicePtr->createObject(scenePtr.get(), objectFactoryPtr.get(), tag1);
        objects = sceneServicePtr->getObjectsByObjectType(scenePtr.get(), "Object");
        EXPECT_EQ(1, objects.size());
        EXPECT_EQ(tag1, objects[0]->getTag());
    }

    TEST_F(SceneServiceTest, TestRemoveObject){
        std::string tag1 = "tag1";
        std::unique_ptr<ISceneService> sceneServicePtr(new SceneService(loggerFactoryPtr.get()));
        Object *object = sceneServicePtr->createObject(scenePtr.get(), objectFactoryPtr.get(), tag1);
        sceneServicePtr->removeObject(scenePtr.get(), objectFactoryPtr.get(), object);
        EXPECT_TRUE(sceneServicePtr->isTagAvailable(scenePtr.get(), tag1));
        std::vector<Object*> objects = sceneServicePtr->getObjectsByObjectType(scenePtr.get(), "Object");
        EXPECT_EQ(0, objects.size());
        Object *objectRetrieved = sceneServicePtr->getObjectByTag(scenePtr.get(), tag1);
        EXPECT_EQ(nullptr, objectRetrieved);
    }

    TEST_F(SceneServiceTest, TestDuplicateTagDuplication){
        std::string tag1 = "tag1";
        std::unique_ptr<ISceneService> sceneServicePtr(new SceneService(loggerFactoryPtr.get()));

        Object *object = sceneServicePtr->createObject(scenePtr.get(), objectFactoryPtr.get(), tag1);
        EXPECT_NE(nullptr, object);
        EXPECT_EQ(tag1, object->getTag());

        Object *repetedObject = sceneServicePtr->createObject(scenePtr.get(), objectFactoryPtr.get(), tag1);
        EXPECT_EQ(nullptr, repetedObject);
        
        EXPECT_FALSE(sceneServicePtr->isTagAvailable(scenePtr.get(), tag1));
        std::vector<Object*> objects = sceneServicePtr->getObjectsByObjectType(scenePtr.get(), "Object");
        EXPECT_EQ(1, objects.size());
        Object *objectRetrieved = sceneServicePtr->getObjectByTag(scenePtr.get(), tag1);
        EXPECT_NE(nullptr, objectRetrieved);
    }
}