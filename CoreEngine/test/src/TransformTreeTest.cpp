#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <coreEngine/components/transformTree/TransformTree.h>

namespace cl{
	TEST(TransformTreeTest, TestElementAdditionAndRemoval){
		std::unique_ptr<TransformTree> parent(new TransformTree);
		EXPECT_EQ(nullptr, parent->getParent());
		EXPECT_EQ(0, parent->getChildren().size());
		std::unique_ptr<TransformTree> child1(new TransformTree);
		
		parent->addChild(child1.get());
		EXPECT_NE(nullptr, child1->getParent());
		EXPECT_EQ(1, parent->getChildren().size());

		std::unique_ptr<TransformTree> child2(new TransformTree);

		parent->addChild(child2.get());
		EXPECT_NE(nullptr, child1->getParent());
		EXPECT_EQ(2, parent->getChildren().size());

		parent->deleteChild(child1.get());
		EXPECT_EQ(1, parent->getChildren().size());

		parent->deleteChild(child1.get());
		EXPECT_EQ(1, parent->getChildren().size());

		parent->deleteChild(child2.get());
		EXPECT_EQ(0, parent->getChildren().size());

		parent->deleteChild(child2.get());
		EXPECT_EQ(0, parent->getChildren().size());
	}
}