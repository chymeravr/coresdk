#include <gtest/gtest.h>
#include <coreEngineTest/mocks/LoggerFactoryMock.h>
#include <coreEngineTest/mocks/RelationMock.h>

namespace cl{
	class RelationTest : public ::testing::Test{
	protected:
		virtual void SetUp(){
			loggerFactory = std::unique_ptr<ILoggerFactory>(new LoggerFactoryMock);
			relation1uptr = std::unique_ptr<Relation>(new RelationMock(id1, type1, loggerFactory.get()));
			relation1 = relation1uptr.get();
			relation11uptr = std::unique_ptr<Relation>(new RelationMock(id11, type11, loggerFactory.get()));
			relation11 = relation11uptr.get();
			relation2uptr = std::unique_ptr<Relation>(new RelationMock(id2, type2, loggerFactory.get()));
			relation2 = relation2uptr.get();
			relation21uptr = std::unique_ptr<Relation>(new RelationMock(id21, type21, loggerFactory.get()));
			relation21 = relation21uptr.get();
		}
		virtual void TearDown(){
		}
		std::unique_ptr<ILoggerFactory> loggerFactory;

		std::string type1 = "type1";
		std::string id1 = "id1";
		std::unique_ptr<Relation> relation1uptr;
		Relation *relation1;

		std::string type11 = "type1";
		std::string id11 = "id11";
		std::unique_ptr<Relation> relation11uptr;
		Relation *relation11;

		std::string type2 = "type2";
		std::string id2 = "id2";
		std::unique_ptr<Relation> relation2uptr;
		Relation *relation2;

		std::string type21 = "type2";
		std::string id21 = "id21";
		std::unique_ptr<Relation> relation21uptr;
		Relation *relation21;
	};

	TEST_F(RelationTest, TestEmptyRelation){
		EXPECT_DEATH(relation1->exists(nullptr), "");
		EXPECT_FALSE(relation1->exists(relation2));
		std::vector<Relation*> targetRelations = relation1->getRelations(relation2->getType());
		EXPECT_EQ(0, targetRelations.size());
	}

	TEST_F(RelationTest, TestRelationCreation){
		EXPECT_DEATH(relation1->createRelation(nullptr), "");
		relation1->createRelation(relation2);
		EXPECT_FALSE(relation2->exists(relation1));
		EXPECT_TRUE(relation1->exists(relation2));
		std::vector<Relation*> targetRelations = relation1->getRelations(relation2->getType());
		EXPECT_EQ(1, targetRelations.size());
		EXPECT_EQ(id2, targetRelations[0]->getUniqueIdentifier());
	}

	TEST_F(RelationTest, TestDuplicateCreationDetection){
		relation1->createRelation(relation2);
		EXPECT_DEATH(relation1->createRelation(relation2), "");
	}

	TEST_F(RelationTest, TestMultipleCreation){
		relation1->createRelation(relation2);
		EXPECT_TRUE(relation1->exists(relation2));
		EXPECT_FALSE(relation1->exists(relation21));
		relation1->createRelation(relation21);
		EXPECT_TRUE(relation1->exists(relation21));
		std::vector<Relation*> targetRelations = relation1->getRelations(relation2->getType());
		EXPECT_EQ(2, targetRelations.size());
		EXPECT_EQ(type2, targetRelations[0]->getType());
		EXPECT_EQ(type2, targetRelations[1]->getType());
		EXPECT_NE(targetRelations[0]->getUniqueIdentifier(), targetRelations[1]->getUniqueIdentifier());
	}

	TEST_F(RelationTest, TestDestroyRelation){
		EXPECT_DEATH(relation1->destroyRelation(nullptr), "");
		EXPECT_DEATH(relation1->destroyRelation(relation2), "");
		relation1->createRelation(relation2);
		relation1->createRelation(relation21);
		relation1->destroyRelation(relation2);
		EXPECT_FALSE(relation1->exists(relation2));
		std::vector<Relation*> targetRelations = relation1->getRelations(relation2->getType());
		EXPECT_EQ(1, targetRelations.size());
		EXPECT_EQ(id21, targetRelations[0]->getUniqueIdentifier());
		relation1->destroyRelation(relation21);
		EXPECT_FALSE(relation1->exists(relation21));
		targetRelations = relation1->getRelations(relation21->getType());
		EXPECT_EQ(0, targetRelations.size());
	}

	TEST_F(RelationTest, TestBiRelationCreation){
		relation1->createBiRelation(relation2);
		EXPECT_TRUE(relation1->exists(relation2));
		EXPECT_TRUE(relation2->exists(relation1));
		std::vector<Relation*> relationsOf1 = relation1->getRelations(type2);
		EXPECT_EQ(1, relationsOf1.size());
		EXPECT_EQ(id2, relationsOf1[0]->getUniqueIdentifier());
		std::vector<Relation*> relationsOf2 = relation2->getRelations(type1);
		EXPECT_EQ(1, relationsOf2.size());
		EXPECT_EQ(id1, relationsOf2[0]->getUniqueIdentifier());

		//Test duplication detection
		EXPECT_DEATH(relation1->createBiRelation(relation2), "");

		//Test conversion from uni to bi relation detection
		relation1->createRelation(relation21);
		EXPECT_DEATH(relation1->createBiRelation(relation21), "");
		EXPECT_DEATH(relation21->createBiRelation(relation1), "");
		relation1->destroyRelation(relation21);

		//Test multiple creation
		relation21->createBiRelation(relation1);
		EXPECT_TRUE(relation1->exists(relation21));
		EXPECT_TRUE(relation21->exists(relation1));
		relationsOf1 = relation1->getRelations(type2);
		EXPECT_EQ(2, relationsOf1.size());
		EXPECT_NE(relationsOf1[0]->getUniqueIdentifier(), relationsOf1[1]->getUniqueIdentifier());
		std::vector<Relation*> relationsOf21 = relation21->getRelations(type1);
		EXPECT_EQ(1, relationsOf21.size());
		EXPECT_EQ(id1, relationsOf21[0]->getUniqueIdentifier());
	}

	TEST_F(RelationTest, TestBiRelationDestroy){
		EXPECT_DEATH(relation1->destroyBiRelation(nullptr), "");
		EXPECT_DEATH(relation1->destroyBiRelation(relation2), "");

		relation1->createRelation(relation2);
		EXPECT_DEATH(relation1->destroyBiRelation(relation2), "");
		EXPECT_DEATH(relation2->destroyBiRelation(relation1), "");
		relation1->destroyRelation(relation2);

		relation1->createBiRelation(relation2);
		relation1->createBiRelation(relation21);

		relation2->destroyBiRelation(relation1);
		EXPECT_FALSE(relation1->exists(relation2));
		EXPECT_FALSE(relation2->exists(relation1));
		EXPECT_TRUE(relation1->exists(relation21));
		EXPECT_TRUE(relation21->exists(relation1));

		relation1->destroyBiRelation(relation21);
		EXPECT_FALSE(relation1->exists(relation21));
		EXPECT_FALSE(relation21->exists(relation1));
	}
}