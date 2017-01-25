#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(Testsadf, abdsfTEst){
    EXPECT_EQ(1, 1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);
    int status = RUN_ALL_TESTS();
    //std::cout << "Press an integer to close the window" << std::endl;
    //int a;
    //std::cin >> a;
    return status;
}
