#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "../r3.hpp"
#include "gtest/gtest.h"
#include "test_class.hpp"


/* single-threaded tests */
// FUTURE: make sure all tests work for arrays as well
// TODO: templatize these tests

void unit_test::make_recycle_memory(std::vector<size_t> shape, int max) {
// -> check correct number of free
    recycle_memory<int> r(shape, max);

    EXPECT_EQ(r.private_free_size(), max) << "Free list did not make " 
        + std::to_string(max) + " buffers.";
}

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
    {
        buffer_ptr<int> p = recycler->fill();

        EXPECT_EQ(recycler->private_free_size(),  max - 1) << "Expected " + 
            std::to_string(max - 1) + " free buffers. Have " + 
            std::to_string(recycler->private_free_size());

        EXPECT_EQ(p.use_count(), 1) << "Too many shared_pointer owners.";
    }

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
    ASSERT_EQ(b.use_count(), 1) << 
        "Unexpected reference count. Expected 1 and got " + 
        std::to_string(b.use_count());

    recycler->queue(b);

    EXPECT_EQ(recycler->private_free_size(), max - 1) << "Expected " + 
        std::to_string(max - 1) + " free buffers. Have " + 
        std::to_string(recycler->private_free_size());
    

    EXPECT_EQ(recycler->private_queue_size(), 1) << 
        "Expected 1 queued buffer. Have " + 
        std::to_string(recycler->private_queue_size());
    

    EXPECT_EQ(b.use_count(), 2) << 
        "Unexpected reference count. Expected 2 and got " + 
        std::to_string(b.use_count());
    // one count for this function, one count for the queue
    
}

// FUTURE: array access

// FUTURE: if user can give own shared_ptrs, check queue has max length

/* Thread safety/ concurrency tests */

void thread_read(buffer_ptr<int> b, int data) {
    EXPECT_GE(b.use_count(), 2) << 
        "Unexpected reference count. Expected at least 2 and got " + 
        std::to_string(b.use_count());
    
    EXPECT_EQ(*b, data) << "Unexpected value. Expected " + 
        std::to_string(data) + " but got " + std::to_string(*b);
    
}

void unit_test::change_buffer_threaded(
    std::shared_ptr<recycle_memory<int>> recycler
) {
    // -> check new data can be accessed from same & another shared_ptr instance (thread)

    auto b = recycler->fill();
    *b = 5;
    ASSERT_EQ(b.use_count(), 1) << 
        "Unexpected reference count. Expected 1 and got " + 
        std::to_string(b.use_count());
    ASSERT_EQ(*b, 5) << "Unexpected value. Expected " + 
        std::to_string(5) + " but got " + std::to_string(*b);

    std::thread check(thread_read, b, 5);

    check.join();

    EXPECT_EQ(b.use_count(), 1) << 
        "Unexpected reference count. Expected 1 and got " + 
        std::to_string(b.use_count());
}

void thread_wait_fill(
    std::shared_ptr<recycle_memory<int>> recycler, 
    std::shared_ptr<std::condition_variable> cv,
    bool &waiting_unsafe
) {
    waiting_unsafe = true;
    cv->notify_one();
    auto b = recycler->fill();

    EXPECT_EQ(b.use_count(), 1) << 
        "Unexpected reference count. Expected 1 and got " + 
        std::to_string(b.use_count());
    
    waiting_unsafe = false;
}

//NOTE: max must be 1 for this
void unit_test::wait_take_from_fill_threaded(
    std::shared_ptr<recycle_memory<int>> recycler,
    int max
) {
// -> check that the last one waits
    bool waiting_unsafe = false;
    std::thread check;
    {
        auto b = recycler->fill();
        
        std::shared_ptr<std::condition_variable> cv = std::make_shared<std::condition_variable>();
        std::mutex m;

        ASSERT_EQ(b.use_count(), 1) << 
            "Unexpected reference count. Expected 1 and got " + 
            std::to_string(b.use_count());

        check = std::thread(thread_wait_fill, recycler, cv, std::ref(waiting_unsafe));

        std::unique_lock<std::mutex> lk(m);
        while(!waiting_unsafe) {
            cv->wait(lk);
        }

        *b = 5; // this needs to take long enough for the thread to set waiting_unsafe
        ASSERT_EQ(*b, 5) << "Unexpected value. Expected " + 
            std::to_string(5) + " but got " + std::to_string(*b);
        
        ASSERT_EQ(b.use_count(), 1) << 
            "Unexpected reference count. Expected 1 and got " + 
            std::to_string(b.use_count());

        ASSERT_TRUE(waiting_unsafe) << 
            "Thread did not wait for buffer to be available";

        EXPECT_EQ(b.use_count(), 1) << 
            "Unexpected reference count. Expected 1 and got " + 
            std::to_string(b.use_count());
    }

    check.join();

    EXPECT_FALSE(waiting_unsafe) << 
        "Thread did not end and join correctly";

    EXPECT_EQ(recycler->private_free_size(), max) << "Expected " + 
        std::to_string(max) + " free buffers. Have " + 
        std::to_string(recycler->private_free_size());
}


void thread_wait_queue(
    std::shared_ptr<recycle_memory<int>> recycler, 
    std::shared_ptr<std::condition_variable> cv,
    bool &waiting_unsafe
) {
    waiting_unsafe = true;
    cv->notify_one();
    auto b = recycler->operate();
    EXPECT_EQ(b.use_count(), 1) << 
        "Unexpected reference count. Expected 1 and got " + 
        std::to_string(b.use_count());
    waiting_unsafe = false;

}

void unit_test::buffer_from_empty_queue_threaded(
    std::shared_ptr<recycle_memory<int>> recycler,
    int max
) {
// -> check that it waits, 
// -> THEN it takes available buffer once queued, check buffer is same

    bool waiting_unsafe = false;
    std::shared_ptr<std::condition_variable> cv = std::make_shared<std::condition_variable>();
    std::mutex m;

    std::thread check(thread_wait_queue, recycler, cv, std::ref(waiting_unsafe));

    std::unique_lock<std::mutex> lk(m);
    while(!waiting_unsafe) {
        cv->wait(lk);
    }

    ASSERT_TRUE(waiting_unsafe) << 
        "Thread did not wait for buffer to be available";

    {
        auto b = recycler->fill();
        *b = 5;
        ASSERT_EQ(b.use_count(), 1) << 
            "Unexpected reference count. Expected 1 and got " + 
            std::to_string(b.use_count());
        recycler->queue(b);
    }

    check.join();

    EXPECT_FALSE(waiting_unsafe) << 
        "Thread did not end and join correctly";

    EXPECT_EQ(recycler->private_free_size(), max) << "Expected " + 
        std::to_string(max) + " free buffers. Have " + 
        std::to_string(recycler->private_free_size());
}

// TODO: exercise reference counting with the buffer & recycle (use a watcher)
// TODO: exercise thread waiting for buffer max+1


// exercise waiting for operation buffer
// TODO: exercise multiple writing threads
// exercise passing by const, check reference count, check not able to change buffer

// exercise passing pointer to free list, with buffer destruction
// exercise destruction of pointer (this might just be a valgrind test)
// exercise releasing buffer after "fill", no "queue" (have a watcher with ref count)
