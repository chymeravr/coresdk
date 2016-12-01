#include <gtest/gtest.h>
#include <coreEngine/service/defaultImpl/ComponentService.h>
#include <coreEngine/factory/defaultImpl/ObjectFactory.h>
#include <coreEngine/model/ComponentStore.h>
#include <coreEngineTest/mocks/LoggerFactoryMock.h>
#include <coreEngineTest/mocks/ComponentMock.h>

namespace cl{
    class ComponentServiceTest : public ::testing::Test{
    protected:
        std::unique_ptr<Object> objectPtr;
        std::string objectTag = "objectTag";
        std::unique_ptr<IComponentService> componentServicePtr;
        std::unique_ptr<ILoggerFactory> loggerFactoryPtr;
        virtual void SetUp(){
            std::unique_ptr<IObjectFactory> objectFactoryPtr(new ObjectFactory());
            objectPtr = std::unique_ptr<Object> (objectFactoryPtr->createObject(objectTag));
            loggerFactoryPtr = std::unique_ptr<ILoggerFactory>(new LoggerFactoryMock);
            componentServicePtr = std::unique_ptr<IComponentService>(new ComponentService(loggerFactoryPtr.get()));
        }
        virtual void TearDown(){

        }
    };

    TEST_F(ComponentServiceTest, TestAddComponentToComponentStore){
        std::unique_ptr<IComponent> componentPtr(new ComponentMock());
        std::string componentType = componentPtr->getComponentType();
        ComponentStore componentStore;
        {
            componentServicePtr->addComponentToComponentStore(&componentStore, std::move(componentPtr), objectPtr.get());
            std::unordered_map<std::string, std::unordered_map<std::string, std::unique_ptr<IComponent> > > &componentsByTypeByObjectTag = componentStore.getComponentsByTypeByObjectTag();
            EXPECT_EQ(1, componentsByTypeByObjectTag.size());
            EXPECT_EQ(1, componentsByTypeByObjectTag[objectPtr->getTag()].size());
            IComponent *retComponentPtr = componentsByTypeByObjectTag[objectPtr->getTag()][componentType].get();
            EXPECT_NE(nullptr, retComponentPtr);
            EXPECT_EQ(componentType, retComponentPtr->getComponentType());

            std::unordered_map<std::string, std::vector<Object*> > &objectsByComponentType = componentStore.getObjectsByComponentType();
            EXPECT_EQ(1, objectsByComponentType.size());
            std::vector<Object*> &objects = objectsByComponentType[componentType];
            EXPECT_EQ(1, objects.size());
            EXPECT_NE(nullptr, objects[0]);
            EXPECT_EQ(objectPtr->getTag(), objects[0]->getTag());
        }

        //checking if duplication causes any trouble
        {
            std::unique_ptr<IComponent> componentPtr(new ComponentMock());
            std::string componentType = componentPtr->getComponentType();
            componentServicePtr->addComponentToComponentStore(&componentStore, std::move(componentPtr), objectPtr.get());
            std::unordered_map<std::string, std::unordered_map<std::string, std::unique_ptr<IComponent> > > &componentsByTypeByObjectTag = componentStore.getComponentsByTypeByObjectTag();
            EXPECT_EQ(1, componentsByTypeByObjectTag.size());
            EXPECT_EQ(1, componentsByTypeByObjectTag[objectPtr->getTag()].size());
            IComponent *retComponentPtr = componentsByTypeByObjectTag[objectPtr->getTag()][componentType].get();
            EXPECT_NE(nullptr, retComponentPtr);
            EXPECT_EQ(componentType, retComponentPtr->getComponentType());

            std::unordered_map<std::string, std::vector<Object*> > &objectsByComponentType = componentStore.getObjectsByComponentType();
            EXPECT_EQ(1, objectsByComponentType.size());
            std::vector<Object*> &objects = objectsByComponentType[componentType];
            EXPECT_EQ(1, objects.size());
            EXPECT_NE(nullptr, objects[0]);
            EXPECT_EQ(objectPtr->getTag(), objects[0]->getTag());
        }
    }

    TEST_F(ComponentServiceTest, TestIsComponentTypeExists){
        std::unique_ptr<IComponent> componentPtr(new ComponentMock());
        std::string componentType = componentPtr->getComponentType();
        ComponentStore componentStore;
        EXPECT_FALSE(componentServicePtr->isComponentTypeExists(objectPtr.get(), componentType));
        componentServicePtr->addComponentToComponentStore(&componentStore, std::move(componentPtr), objectPtr.get());
        EXPECT_TRUE(componentServicePtr->isComponentTypeExists(objectPtr.get(), componentType));
    }

    TEST_F(ComponentServiceTest, TestRemoveComponentsFromComponentStore){
        std::unique_ptr<IComponent> componentPtr(new ComponentMock());
        std::string componentType = componentPtr->getComponentType();
        IComponent *copiedComponentPtr = componentPtr.get();
        ComponentStore componentStore;
        componentServicePtr->addComponentToComponentStore(&componentStore, std::move(componentPtr), objectPtr.get());
        componentServicePtr->removeComponentFromComponentStore(&componentStore, copiedComponentPtr);
        EXPECT_FALSE(componentServicePtr->isComponentTypeExists(objectPtr.get(), componentType));

        {
            std::unordered_map<std::string, std::unordered_map<std::string, std::unique_ptr<IComponent> > > &componentsByTypeByObjectTag = componentStore.getComponentsByTypeByObjectTag();
            EXPECT_EQ(1, componentsByTypeByObjectTag.size());
            EXPECT_EQ(0, componentsByTypeByObjectTag[objectPtr->getTag()].size());
            
            std::unordered_map<std::string, std::vector<Object*> > &objectsByComponentType = componentStore.getObjectsByComponentType();
            EXPECT_EQ(1, objectsByComponentType.size());
            std::vector<Object*> &objects = objectsByComponentType[componentType];
            EXPECT_EQ(0, objects.size());
        }
    }

    TEST_F(ComponentServiceTest, TestGetComponent){
        std::unique_ptr<IComponent> componentPtr(new ComponentMock());
        std::string componentType = componentPtr->getComponentType();
        IComponent *copiedComponentPtr = componentPtr.get();
        ComponentStore componentStore;
        componentServicePtr->addComponentToComponentStore(&componentStore, std::move(componentPtr), objectPtr.get());
        {
            IComponent *gotComponentPtr = componentServicePtr->getComponent(objectPtr.get(), componentType);
            EXPECT_NE(nullptr, gotComponentPtr);
            EXPECT_EQ(componentType, gotComponentPtr->getComponentType());
        }
        componentServicePtr->removeComponentFromComponentStore(&componentStore, copiedComponentPtr);
        {
            IComponent *gotComponentPtr = componentServicePtr->getComponent(objectPtr.get(), componentType);
            EXPECT_EQ(nullptr, gotComponentPtr);
        }
    }

    TEST_F(ComponentServiceTest, TestGetComponents){
        std::unique_ptr<IComponent> componentPtr(new ComponentMock());
        std::string componentType = componentPtr->getComponentType();
        IComponent *copiedComponentPtr = componentPtr.get();
        ComponentStore componentStore;
        componentServicePtr->addComponentToComponentStore(&componentStore, std::move(componentPtr), objectPtr.get());
        {
            std::vector<IComponent*> components = componentServicePtr->getComponents(objectPtr.get());
            EXPECT_EQ(1, components.size());
        }
        componentServicePtr->removeComponentFromComponentStore(&componentStore, copiedComponentPtr);
        {
            std::vector<IComponent*> components = componentServicePtr->getComponents(objectPtr.get());
            EXPECT_EQ(0, components.size());
        }
    }

    TEST_F(ComponentServiceTest, TestGetObjectsByComponentType){
        std::unique_ptr<IComponent> componentPtr(new ComponentMock());
        std::string componentType = componentPtr->getComponentType();
        IComponent *copiedComponentPtr = componentPtr.get();
        ComponentStore componentStore;
        componentServicePtr->addComponentToComponentStore(&componentStore, std::move(componentPtr), objectPtr.get());
        {
            std::vector<Object*> objects = componentServicePtr->getObjectsByComponentType(&componentStore, componentType);
            EXPECT_EQ(1, objects.size());
            EXPECT_EQ(objectPtr->getTag(), objects[0]->getTag());
        }
        componentServicePtr->removeComponentFromComponentStore(&componentStore, copiedComponentPtr);
        {
            std::vector<Object*> objects = componentServicePtr->getObjectsByComponentType(&componentStore, componentType);
            EXPECT_EQ(0, objects.size());
        }
    }
}