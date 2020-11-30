//
// Created by ludwig on 22.11.16.
//


#include "Tests.h"


//TEST(HELLO, FIRST_STEPS) {
//    std::cout << "Hello world!" << std::endl;
//    // Google Test will also provide macros for assertions.
//    ASSERT_EQ(1+1, 2);
//}




int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
