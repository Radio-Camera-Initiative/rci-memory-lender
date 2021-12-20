#include "gtest/gtest.h"
#include "test_class.hpp"
#include "../r3.hpp"

TEST(BasicFunctions, MakeRecycleMemory) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    unit_test::make_recycle_memory(shape, max);
}

TEST(BasicFunctions, TakeWithFill) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<int>> r3 = std::make_shared<recycle_memory<int>>(shape, max); 
    unit_test::take_one_buffer_from_fill(r3, shape, max);
}