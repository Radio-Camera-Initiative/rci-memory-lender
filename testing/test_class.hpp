
#ifndef test_class_H
#define test_class_H

#include <vector>
#include <memory>

#include "../r3.hpp"

/* Unit testing class - for friending purposes */
struct unit_test {
    unit_test() {}

    static void make_recycle_memory(std::vector<size_t> shape, int max);
    static void take_one_buffer_from_fill(std::shared_ptr<recycle_memory<int>> recycler, std::vector<size_t> shape, int max);
    static void check_buffer_destruction(std::shared_ptr<recycle_memory<int>> recycler, int max);
    static void change_one_buffer(std::shared_ptr<recycle_memory<int>> recycler, int max, int data);
    static void queue_buffer_from_fill (std::shared_ptr<recycle_memory<int>> recycler, int max);

    static void change_buffer_threaded(std::shared_ptr<recycle_memory<int>> recycler);
    static void wait_take_from_fill_threaded(std::shared_ptr<recycle_memory<int>> recycler, int max);
    static void buffer_from_empty_queue_threaded(std::shared_ptr<recycle_memory<int>> recycler, int max);

};

#endif