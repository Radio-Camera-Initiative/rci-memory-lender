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

// make reuseable buffer
// -> check shape is as expected, data is as expected

// changing data in pointer
// -> check new data can be accessed from same & another shared_ptr instance (thread)

// FUTURE: array access

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


// take max + 1 buffers from fill 
// -> check that the last one waits

// ask for buffer from empty queue
// -> check that it waits, 
// -> THEN it takes available buffer once queued, check buffer is same

// queue one buffer (from fill) 
// -> check number of free, number of queue, number of shared_ptr refs

// release all buffer references
// -> check that the free queue is full again ie max

// FUTURE: if user can give own shared_ptrs, check queue has max length

/* Thread safety/ concurrency tests */

// TODO: exercise reference counting with the buffer & recycle (use a watcher)
// TODO: exercise thread waiting for buffer max+1


// exercise waiting for operation buffer
// TODO: exercise multiple writing threads
// exercise passing by const, check reference count, check not able to change buffer

// exercise passing pointer to free list, with buffer destruction
// exercise destruction of pointer (this might just be a valgrind test)
// exercise releasing buffer after "fill", no "queue" (have a watcher with ref count)
