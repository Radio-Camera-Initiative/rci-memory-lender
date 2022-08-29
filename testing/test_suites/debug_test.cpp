#include "gtest/gtest.h"
#include "test_class.hpp"
#include "../lender.hpp"
#include <complex>
#include <numeric>

#ifndef NDEBUG

TEST(Debug, MemoryCheckZero) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<int>> r3 = 
        std::make_shared<library<int>>(shape, max); 
    {
        auto buffer = r3->fill();
        EXPECT_EQ(*buffer, 0);
        *buffer = 1;
    }
    auto buffer = r3->fill();
    EXPECT_EQ(*buffer, 0);
}

TEST(Debug, MemoryCheckF0) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<int>> r3 = 
        std::make_shared<library<int>>(shape, max); 
    int* raw_ptr;
    {
        auto buffer = r3->fill();
        ASSERT_EQ(*buffer, 0);
        raw_ptr = buffer.get();
    }

    int* f = new int();
    memset(f, 0xf0, sizeof(int));
    EXPECT_EQ(*f, *raw_ptr);
    free(f);
}

TEST(Debug, MemoryCheckUseAfterFreeAssert) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<int>> r3 = 
        std::make_shared<library<int>>(shape, max); 
    int* raw_ptr;
    {
        auto buffer = r3->fill();
        ASSERT_EQ(*buffer, 0);
        raw_ptr = buffer.get();
    }

    *raw_ptr = 1;

    EXPECT_DEATH(r3->fill(), "Assertion .* failed.");
}

TEST(Debug, QueueInvalidPointerDiffRecycler) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<int>> r3 = 
        std::make_shared<library<int>>(shape, max); 
    std::shared_ptr<library<int>> r2 = 
        std::make_shared<library<int>>(shape, max); 

    auto p2 = r2->fill();
    EXPECT_DEATH(r3->queue(p2), ".* Assertion .* failed.");
}

#endif