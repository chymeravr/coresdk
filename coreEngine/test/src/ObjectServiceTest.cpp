#include <memory>
#include <gtest/gtest.h>
#include <coreEngine/service/IObjectService.h>
#include <coreEngine/service/defaultImpl/ObjectService.h>
#include <coreEngine/factory/defaultImpl/ObjectFactory.h>

#include <coreEngineTest/mocks/LoggerMock.h>
#include <coreEngineTest/mocks/LoggerFactoryMock.h>

namespace cl{

	TEST(ObjectServiceTest, TestLinkObjectAndLinkedObjectGetters){
		// Creating objects
		std::string type = "Object";
		std::string tag1 = "Tag1";
		std::string tag2 = "Tag2";
		ObjectFactory objectFactory;
		std::unique_ptr<Object> objectPtr1 = objectFactory.createObject(tag1);
		std::unique_ptr<Object> objectPtr2 = objectFactory.createObject(tag2);

		// Creating service
		std::unique_ptr<ILoggerFactory> loggerFactoryPtr(new LoggerFactoryMock());
        std::unique_ptr<IObjectService> objectServicePtr(new ObjectService(loggerFactoryPtr.get()));

		// Testing pre-linkage
		Object *gotObject2 = objectServicePtr->getLinkedObjectByTag(objectPtr1.get(), tag2);
		Object *gotObject1 = objectServicePtr->getLinkedObjectByTag(objectPtr2.get(), tag1);
		EXPECT_EQ(nullptr, gotObject2);
		EXPECT_EQ(nullptr, gotObject1);
		std::vector<Object *> gotObjectsVec2 = objectServicePtr->getLinkedObjectsByObjectType(objectPtr1.get(), type);
		std::vector<Object *> gotObjectsVec1 = objectServicePtr->getLinkedObjectsByObjectType(objectPtr2.get(), type);
		EXPECT_EQ(0, gotObjectsVec2.size());
		EXPECT_EQ(0, gotObjectsVec1.size());
		//linkage
		objectServicePtr->linkObject(objectPtr1.get(), objectPtr2.get());

		//Post linkage testing
		gotObject2 = objectServicePtr->getLinkedObjectByTag(objectPtr1.get(), tag2);
		EXPECT_NE(nullptr, gotObject2);
		EXPECT_EQ(tag2, gotObject2->getTag());

		gotObject1 = objectServicePtr->getLinkedObjectByTag(objectPtr2.get(), tag1);
		EXPECT_NE(nullptr, gotObject1);
		EXPECT_EQ(tag1, gotObject1->getTag());

		//Testing using type of linked object

		gotObjectsVec2 = objectServicePtr->getLinkedObjectsByObjectType(objectPtr1.get(), type);
		gotObjectsVec1 = objectServicePtr->getLinkedObjectsByObjectType(objectPtr2.get(), type);
		EXPECT_EQ(1, gotObjectsVec2.size());
		EXPECT_EQ(1, gotObjectsVec1.size());

		EXPECT_EQ(tag2, gotObjectsVec2[0]->getTag());
		EXPECT_EQ(tag1, gotObjectsVec1[0]->getTag());

	}

	TEST(ObjectServiceTest, TestSameObjectLinking){
		// Creating objects
		std::string type = "Object";
		std::string tag = "Tag";
		ObjectFactory objectFactory;
		std::unique_ptr<Object> objectPtr1 = objectFactory.createObject(tag);

		// Creating service
		std::unique_ptr<ILoggerFactory> loggerFactoryPtr(new LoggerFactoryMock());
		std::unique_ptr<IObjectService> objectServicePtr(new ObjectService(loggerFactoryPtr.get()));

		objectServicePtr->linkObject(objectPtr1.get(), objectPtr1.get());

		Object *gotObject = objectServicePtr->getLinkedObjectByTag(objectPtr1.get(), tag);
		EXPECT_EQ(nullptr, gotObject);

		std::vector<Object*> gotObjectVec = objectServicePtr->getLinkedObjectsByObjectType(objectPtr1.get(), type);
		EXPECT_EQ(0, gotObjectVec.size());
	}

	TEST(ObjectServiceTest, TestUnlinkObject){
		// Creating objects
		std::string type = "Object";
		std::string tag1 = "Tag1";
		std::string tag2 = "Tag2";
		ObjectFactory objectFactory;
		std::unique_ptr<Object> objectPtr1 = objectFactory.createObject(tag1);
		std::unique_ptr<Object> objectPtr2 = objectFactory.createObject(tag2);

		// Creating service
        std::unique_ptr<ILoggerFactory> loggerFactoryPtr(new LoggerFactoryMock());
        std::unique_ptr<IObjectService> objectServicePtr(new ObjectService(loggerFactoryPtr.get()));

		//linkage
		objectServicePtr->linkObject(objectPtr1.get(), objectPtr2.get());

		//unlinkage
		objectServicePtr->unlinkObject(objectPtr1.get(), objectPtr2.get());

		// Testing if unlinkage is fine
		Object *gotObject2 = objectServicePtr->getLinkedObjectByTag(objectPtr1.get(), tag2);
		Object *gotObject1 = objectServicePtr->getLinkedObjectByTag(objectPtr2.get(), tag1);
		EXPECT_EQ(nullptr, gotObject2);
		EXPECT_EQ(nullptr, gotObject1);
		std::vector<Object *> gotObjectsVec2 = objectServicePtr->getLinkedObjectsByObjectType(objectPtr1.get(), type);
		std::vector<Object *> gotObjectsVec1 = objectServicePtr->getLinkedObjectsByObjectType(objectPtr2.get(), type);
		EXPECT_EQ(0, gotObjectsVec2.size());
		EXPECT_EQ(0, gotObjectsVec1.size());
	}

	TEST(ObjectServiceTest, TestDuplicateObjectLinking){
		// Creating objects
		std::string type = "Object";
		std::string tag1 = "Tag1";
		std::string tag2 = "Tag2";
		ObjectFactory objectFactory;
		std::unique_ptr<Object> objectPtr1 = objectFactory.createObject(tag1);
		std::unique_ptr<Object> objectPtr2 = objectFactory.createObject(tag2);

		// Creating service
        std::unique_ptr<ILoggerFactory> loggerFactoryPtr(new LoggerFactoryMock());
        std::unique_ptr<IObjectService> objectServicePtr(new ObjectService(loggerFactoryPtr.get()));

		//linkage
		objectServicePtr->linkObject(objectPtr1.get(), objectPtr2.get());

		//duplicate linkage
		objectServicePtr->linkObject(objectPtr1.get(), objectPtr2.get());

		//Post linkage testing
		Object *gotObject2 = objectServicePtr->getLinkedObjectByTag(objectPtr1.get(), tag2);
		EXPECT_NE(nullptr, gotObject2);
		EXPECT_EQ(tag2, gotObject2->getTag());

		Object *gotObject1 = objectServicePtr->getLinkedObjectByTag(objectPtr2.get(), tag1);
		EXPECT_NE(nullptr, gotObject1);
		EXPECT_EQ(tag1, gotObject1->getTag());

		//Testing using type of linked object

		std::vector<Object*> gotObjectsVec2 = objectServicePtr->getLinkedObjectsByObjectType(objectPtr1.get(), type);
		std::vector<Object*>gotObjectsVec1 = objectServicePtr->getLinkedObjectsByObjectType(objectPtr2.get(), type);
		EXPECT_EQ(1, gotObjectsVec2.size());
		EXPECT_EQ(1, gotObjectsVec1.size());

		EXPECT_EQ(tag2, gotObjectsVec2[0]->getTag());
		EXPECT_EQ(tag1, gotObjectsVec1[0]->getTag());
	}

}