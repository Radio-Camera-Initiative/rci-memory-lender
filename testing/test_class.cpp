#include <vector>
#include <memory>

#include "../r3.hpp"
#include "gtest/gtest.h"
#include "test_class.hpp"

/* TODO: is it ok if the user makes a buffer/ gives it memory outside of the 
 * recycle memory, and then uses the queue function?
 */

/* recycle memory tests */
// SETUP: these tests (almost) all need a recycle_memory object
// TODO: make sure all tests work for arrays as well
// TODO: make sure threadsafe

void unit_test::make_recycle_memory(std::vector<size_t> shape, int max) {
// -> check correct number of free
    recycle_memory<int> r(shape, max);

    EXPECT_EQ(r.private_free_size(), max) << "Free list did not make " 
        + std::to_string(max) + " buffers.";
}

// TODO: templatize these tests
void unit_test::take_one_buffer_from_fill(
    std::shared_ptr<recycle_memory<int>> recycler, 
    std::vector<size_t> shape, 
    int max
) {
    // -> check shape, check number of free (max - 1), check num shared_ptr refs
    buffer_ptr<int> p = recycler->fill();    

    for (int i = 0; i < shape.size(); i++) {
        EXPECT_EQ(p->shape[i], shape[i]) << "Shape is incorrect";
    }

    EXPECT_EQ(recycler->private_free_size(),  max - 1) << "Expected " + 
        std::to_string(max - 1) + " free buffers. Have " + 
        std::to_string(recycler->private_free_size());

    EXPECT_EQ(p.use_count(), 1) << "Too many shared_pointer owners.";

}

void unit_test::check_buffer_destruction(
    std::shared_ptr<recycle_memory<int>> recycler, 
    int max
) {
    // -> check number of free is max

    // here we assume that destruction of the buffer was done automatically 
    // when out of scope

    EXPECT_EQ(recycler->private_free_size(), max) << "Expected " + 
        std::to_string(max - 1) + " free buffers. Have " + 
        std::to_string(recycler->private_free_size());

}


void unit_test::change_one_buffer(
    std::shared_ptr<recycle_memory<int>> recycler, 
    int max, 
    int data
) {
    // -> check new data cannot be accessed from another buffer

    auto b1 = recycler->fill();
    auto b2 = recycler->fill();

    *b1 = data;
    *b2 = data;

    ASSERT_EQ(*b1, data) << "Expected b1 to be " + std::to_string(data) + 
        " but got " + std::to_string(*b1);
    ASSERT_EQ(*b2, data) << "Expected b2 to be " + std::to_string(data) + 
        " but got " + std::to_string(*b2);

    *b1 = data + 2;

    EXPECT_EQ(*b1, data+2) << "Expected b1 to be " + std::to_string(data + 2) + 
        " but got " + std::to_string(*b1);
    
    EXPECT_EQ(*b2, data) << "Expected b2 to be " + std::to_string(data) + 
        " but got " + std::to_string(*b2);

}

void unit_test::queue_buffer_from_fill (std::shared_ptr<recycle_memory<int>> recycler, int max) {
// -> check number of free, number of queue, number of shared_ptr refs
    auto b = recycler->fill();

    *b = 5;
    recycler->queue(b);

    EXPECT_EQ(recycler->private_free_size(), max - 1) << "Expected " + 
        std::to_string(max - 1) + " free buffers. Have " + 
        std::to_string(recycler->private_free_size());
    

    EXPECT_EQ(recycler->private_queue_size(), 1) << 
        "Expected 1 queued buffer. Have " + 
        std::to_string(recycler->private_queue_size());
    

    EXPECT_EQ(b.use_count(), 2) << "Too many shared_pointer owners.";
    // one count for this function, one count for the queue
    
}

// FUTURE: array access

// FUTURE: if user can give own shared_ptrs, check queue has max length

/* Thread safety/ concurrency tests */

// void unit_tests::change_buffer_threaded(recycle_memory<int> recycler, int max) {
    // -> check new data can be accessed from same & another shared_ptr instance (thread)
// }

// take max + 1 buffers from fill (threaded)
// -> check that the last one waits

// ask for buffer from empty queue
// -> check that it waits, 
// -> THEN it takes available buffer once queued, check buffer is same

// TODO: exercise reference counting with the buffer & recycle (use a watcher)
// TODO: exercise thread waiting for buffer max+1


// exercise waiting for operation buffer
// TODO: exercise multiple writing threads
// exercise passing by const, check reference count, check not able to change buffer

// exercise passing pointer to free list, with buffer destruction
// exercise destruction of pointer (this might just be a valgrind test)
// exercise releasing buffer after "fill", no "queue" (have a watcher with ref count)
