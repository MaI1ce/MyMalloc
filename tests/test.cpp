#define align8(x) (((((x)-1)>>3)<<3)+8)
#define NULL nullptr
#define SBRK_STUBS
extern "C"{
//#include "../lib/sbrk_stubs.h"
#include "myMalloc.h"
}
#include <gtest/gtest.h>
TEST(MyMallocTest, MallocTest) {

    reset();
/*
    char *ptr1 = (char *)_malloc(-1);
    EXPECT_EQ(ptr1, nullptr);
*/
    char *ptr2 = (char *)_malloc(0);
    EXPECT_EQ(ptr2, nullptr);

    char *ptr3 = (char *)_malloc(1000);
    EXPECT_EQ(ptr3, nullptr);

    char *ptr4 = (char *)_malloc(40);
    ASSERT_EQ(ptr4 - 40, mem);
    ASSERT_EQ(ptr4 + 40, break_ptr);

    char *ptr5 = (char *)_malloc(40);
    ASSERT_EQ(ptr5 - 80, ptr4);
}

TEST(MyMallocTest, CallocTest){
    reset();
/*
    char *ptr1 = (char *)_calloc(-1, -1);
    ASSERT_EQ(ptr1, nullptr);
*/
    char *ptr2 = (char *)_calloc(0, 0);
    ASSERT_EQ(ptr2, nullptr);

    char *ptr3 = (char *)_calloc(0, 1);
    ASSERT_EQ(ptr3, nullptr);

    char *ptr4 = (char *)_calloc(1, 0);
    ASSERT_EQ(ptr4, nullptr);

    char *ptr5 = (char *)_calloc(100, 100);
    ASSERT_EQ(ptr5, nullptr);

    char *ptr6 = (char *)_calloc(10, 10);
    ASSERT_EQ(ptr6 + align8(100), break_ptr);
    ASSERT_EQ(ptr6 - 40, mem);
}

TEST(MyMallocTest, FreeTest){
    reset();

    char *ptr1 = (char *)_malloc(40);
    char *ptr2 = (char *)_malloc(40);
    char *ptr3 = (char *)_malloc(40);

    _free(ptr2);

    char *ptr4 = (char *)_malloc(40);
    ASSERT_EQ(ptr4, ptr2);
    ASSERT_EQ(ptr1 + 80, ptr4);

    _free(ptr4);
    _free(ptr1);
    ASSERT_EQ((size_t)*(ptr1 - 40), 120);

    _free(ptr3);
    ASSERT_EQ(break_ptr, mem);
}

TEST(MyMallocTest, ReallocTest){
    reset();

    char *ptr1 = (char *)_malloc(100);

    char *ptr2 = (char *)_realloc(ptr1, 50);
    EXPECT_EQ(ptr1 + align8(100), break_ptr);
    EXPECT_EQ(ptr1 - 40, mem);
    ASSERT_EQ(ptr1, ptr2);
    ASSERT_EQ((size_t)*(ptr1 - 40), align8(100));


    char *ptr3 = (char *)_realloc(ptr1, 25);
    EXPECT_EQ(ptr1 + align8(100), break_ptr);
    EXPECT_EQ(ptr1 - 40, mem);
    ASSERT_EQ(ptr1, ptr3);
    ASSERT_EQ((size_t)*(ptr1 - 40), align8(25));

    ptr1 = (char *)_realloc(ptr1, 80);
    EXPECT_EQ(ptr1 + align8(100), break_ptr);
    ASSERT_EQ(ptr1 - 40, mem);
    ASSERT_EQ((size_t)*(ptr1 - 40), align8(100));

    char *ptr4 = (char *)_realloc(NULL, 0);
    EXPECT_EQ(ptr4, nullptr);

    char *ptr5 = (char *)_realloc(NULL, 56);
    ASSERT_EQ(ptr5 - 144, ptr1);
    ASSERT_EQ((size_t)*(ptr5 - 40), 56);

    char *ptr6 = (char *)_realloc(ptr1, 0);
    ASSERT_EQ(ptr6, ptr1);

    reset();

    char *ptr7 = (char *)_malloc(40);
    char *ptr8 = (char *)_malloc(40);
    ptr7 = (char *)_realloc(NULL, 56);
    ASSERT_EQ(ptr7, nullptr);

    ptr7 = (char *)_realloc(NULL, 48);
    ASSERT_EQ(break_ptr, mem + 248);

}



 
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}