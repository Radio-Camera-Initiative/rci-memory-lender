#include <vector>
#include <memory>
#include <thread>
#include <string>
#include <complex>
#include <mutex>
#include <condition_variable>

#include "../r3.hpp"
#include "gtest/gtest.h"
#include "test_class.hpp"

template <typename T>
std::string t2string(T t) {
    return std::to_string(t);
}

template <> std::string t2string<std::complex<float>>(std::complex<float> t) {
    return "(" + std::to_string(t.real()) + ", " + std::to_string(t.imag()) + "i)";
}

/* single-threaded tests */
// FUTURE: make sure all tests work for arrays as well
// TODO: templatize these tests

// exercise creation of recycler

template <typename T>
void unit_test::make_recycle_memory(std::vector<size_t> shape, int max) {
// -> check correct number of free
    recycle_memory<T> r(shape, max);

    EXPECT_EQ(r.private_free_size(), max) << "Free list did not make "
        + std::to_string(max) + " buffers.";
}

// exercise reference counts when taking buffer from fill queue

template <typename T>
void unit_test::take_one_buffer_from_fill(
    std::shared_ptr<recycle_memory<T>> recycler,
    std::vector<size_t> shape,
    int max
) {
    // -> check shape, number of free (max - 1), num shared_ptr refs
    buffer_ptr<T> p = recycler->fill();

    for (int i = 0; i < shape.size(); i++) {
        EXPECT_EQ(p->shape[i], shape[i]) << "Shape is incorrect";
    }

    EXPECT_EQ(recycler->private_free_size(),  max - 1) << "Expected " +
        std::to_string(max - 1) + " free buffers. Have " +
        std::to_string(recycler->private_free_size());

    EXPECT_EQ(p.use_count(), 1) << "Too many shared_pointer owners.";

}

// exercise releasing the shared pointer for a buffer and returning memory to
// the recycler

template <typename T>
void unit_test::check_buffer_destruction(
    std::shared_ptr<recycle_memory<T>> recycler,
    int max
) {
    // -> check number of free is max
    {
        buffer_ptr<T> p = recycler->fill();

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

// exercise decrementing buffer reference count

template <typename T>
void unit_test::dec_buffer_ref_count(
    std::shared_ptr<recycle_memory<T>> recycler
) {
    buffer_ptr<T> p = recycler->fill();
    EXPECT_EQ(p.use_count(), 1) << "Too many charge pointer owners.";

    recycler->queue(p);
    EXPECT_EQ(p.use_count(), 2) << "Unexpected reference count";

    {
        buffer_ptr<T> b = recycler->operate();
        EXPECT_EQ(p.use_count(), 2) << "Unexpected reference count";
    }

    EXPECT_EQ(p.use_count(), 1) << "Too many charge pointer owners.";
}

// exercise buffers setting data separately from each other

template <typename T>
void unit_test::change_one_buffer(
    std::shared_ptr<recycle_memory<T>> recycler,
    int max,
    T data
) {
    // -> check new data cannot be accessed from another buffer

    auto b1 = recycler->fill();
    auto b2 = recycler->fill();

    *b1 = data;
    *b2 = data;

    ASSERT_EQ(*b1, data) << "Expected b1 to be " + t2string(data) +
        " but got " + t2string(*b1);
    ASSERT_EQ(*b2, data) << "Expected b2 to be " + t2string(data) +
        " but got " + t2string(*b2);

    *b1 = data + data;

    EXPECT_EQ(*b1, data+data) << "Expected b1 to be " + t2string(data + data) +
        " but got " + t2string(*b1);

    EXPECT_EQ(*b2, data) << "Expected b2 to be " + t2string(data) +
        " but got " + t2string(*b2);

}

// exercise buffers changing values multiple times and it does not interfere 
// with other buffers

template <typename T>
void unit_test::multi_change_buffer(
    std::shared_ptr<recycle_memory<T>> recycler,
    int max,
    T data,
    T diff
) {
    // -> check new data cannot be accessed from another buffer

    auto b1 = recycler->fill();
    auto b2 = recycler->fill();

    *b1 = data;
    *b2 = data;

    ASSERT_EQ(*b1, data) << "Expected b1 to be " + t2string(data) +
        " but got " + t2string(*b1);
    ASSERT_EQ(*b2, data) << "Expected b2 to be " + t2string(data) +
        " but got " + t2string(*b2);

    *b1 = data + diff;

    EXPECT_EQ(*b1, data+diff) << "Expected b1 to be " + t2string(data + diff) +
        " but got " + t2string(*b1);

    EXPECT_EQ(*b2, data) << "Expected b2 to be " + t2string(data) +
        " but got " + t2string(*b2);

    *b2 = data - diff;

    EXPECT_EQ(*b1, data+diff) << "Expected b1 to be " + t2string(data + diff) +
        " but got " + t2string(*b1);

    EXPECT_EQ(*b2, data-diff) << "Expected b2 to be " + t2string(data - diff) +
        " but got " + t2string(*b2);

    *b1 = data;

    EXPECT_EQ(*b1, data) << "Expected b1 to be " + t2string(data) +
        " but got " + t2string(*b1);

    EXPECT_EQ(*b2, data-diff) << "Expected b2 to be " + t2string(data - diff) +
        " but got " + t2string(*b2);

    *b2 = data;

    ASSERT_EQ(*b1, data) << "Expected b1 to be " + t2string(data) +
        " but got " + t2string(*b1);
    ASSERT_EQ(*b2, data) << "Expected b2 to be " + t2string(data) +
        " but got " + t2string(*b2);

}

// exercise setting a buffer (two different ways)

template <typename T>
void unit_test::set_buffer_ptr_array (
    std::shared_ptr<recycle_memory<T>> recycler
) {
    auto buffer = recycler->fill();
    auto shape = buffer->shape;

    size_t size = 1;
    for (auto iter = shape.begin(); iter != shape.end(); iter++) {
        size *= *iter;
    }

    // fill each index with its number
    T i = 0;
    T one = 1;
    for (size_t idx = 0; idx < size; idx++, i+one) {
        buffer[idx] = i;

        ASSERT_EQ(buffer[idx], i) << "Expected index " + std::to_string(idx) +
            " to be " + t2string(i) + " but got " + 
            t2string(buffer[idx]);
    }
}

// exercise using the operating queue for completed (filled) buffers

template <typename T>
void unit_test::queue_buffer_from_fill (
    std::shared_ptr<recycle_memory<T>> recycler, 
    int max,
    T data
) {
// -> check number of free, number of queue, number of shared_ptr refs
    auto b = recycler->fill();

    *b = data;
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

//exercise decrement operate queue (max>1 required)

template <typename T>
void unit_test::dec_operate_queue(
    std::shared_ptr<recycle_memory<T>> recycler
) {
    auto b1 = recycler->fill();
    auto b2 = recycler->fill();

    recycler->queue(b1);
    EXPECT_EQ(recycler->private_queue_size(), 1);
    recycler->queue(b2);
    EXPECT_EQ(recycler->private_queue_size(), 2);

    auto pop1 = recycler->operate();
    EXPECT_EQ(recycler->private_queue_size(), 1);
    auto pop2 = recycler->operate();
    EXPECT_EQ(recycler->private_queue_size(), 0);
}

// FUTURE: array access

// FUTURE: if user can give own shared_ptrs, check queue has max length

/* Thread safety/ concurrency tests */

template <typename T>
void unit_test::thread_read(buffer_ptr<T> b, T data) {
    EXPECT_GE(b.use_count(), 2) <<
        "Unexpected reference count. Expected at least 2 and got " +
        std::to_string(b.use_count());

    EXPECT_EQ(*b, data) << "Unexpected value. Expected " +
        t2string(data) + " but got " + t2string(*b);

}

// change buffer from main thread and check that same data is read from another

template <typename T>
void unit_test::change_buffer_threaded(
    std::shared_ptr<recycle_memory<T>> recycler,
    T data
) {
    // -> check new data accessed from same & threads' shared_ptr instance

    auto b = recycler->fill();
    *b = data;
    ASSERT_EQ(b.use_count(), 1) <<
        "Unexpected reference count. Expected 1 and got " +
        std::to_string(b.use_count());
    ASSERT_EQ(*b, data) << "Unexpected value. Expected " +
        t2string(data) + " but got " + t2string(*b);

    std::thread check(thread_read<T>, b, data);

    check.join();

    EXPECT_EQ(b.use_count(), 1) <<
        "Unexpected reference count. Expected 1 and got " +
        std::to_string(b.use_count());
}

//  change buffer multiple times and check correct data from multiple threads
//  reading same buffer

template <typename T>
void unit_test::multi_change_buffer_threaded(
    std::shared_ptr<recycle_memory<T>> recycler,
    T data[]
) {
    // -> check new data accessed from same & threads' shared_ptr instance

    auto b = recycler->fill();
    *b = data[0];
    ASSERT_EQ(b.use_count(), 1) <<
        "Unexpected reference count. Expected 1 and got " +
        std::to_string(b.use_count());
    ASSERT_EQ(*b, data[0]) << "Unexpected value. Expected " +
        t2string(data[0]) + " but got " + t2string(*b);

    std::thread check_one(thread_read<T>, b, data[0]);
    std::thread check_two(thread_read<T>, b, data[0]);

    EXPECT_GE(b.use_count(), 2) <<
        "Unexpected reference count. Expected at least 2 and got " +
        std::to_string(b.use_count());

    check_one.join();
    check_two.join();

    EXPECT_EQ(b.use_count(), 1) <<
        "Unexpected reference count. Expected 1 and got " +
        std::to_string(b.use_count());

    *b = data[1];
    ASSERT_EQ(*b, data[1]) << "Unexpected value. Expected " +
        t2string(data[1]) + " but got " + t2string(*b);

    std::thread check_three(thread_read<T>, b, data[1]);
    std::thread check_four(thread_read<T>, b, data[1]);

    EXPECT_GE(b.use_count(), 2) <<
        "Unexpected reference count. Expected at least 2 and got " +
        std::to_string(b.use_count());

    check_three.join();
    check_four.join();

    EXPECT_EQ(b.use_count(), 1) <<
        "Unexpected reference count. Expected 1 and got " +
        std::to_string(b.use_count());

    *b = data[2];
    ASSERT_EQ(*b, data[2]) << "Unexpected value. Expected " +
        t2string(data[2]) + " but got " + t2string(*b);

    std::thread check_five(thread_read<T>, b, data[2]);
    std::thread check_six(thread_read<T>, b, data[2]);
    std::thread check_seven(thread_read<T>, b, data[2]);

    EXPECT_GE(b.use_count(), 2) <<
        "Unexpected reference count. Expected at least 2 and got " +
        std::to_string(b.use_count());

    check_five.join();
    check_six.join();
    check_seven.join();

    EXPECT_EQ(b.use_count(), 1) <<
        "Unexpected reference count. Expected 1 and got " +
        std::to_string(b.use_count());
}

template <typename T>
void unit_test::thread_wait_fill(
    std::shared_ptr<recycle_memory<T>> recycler,
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

// exercise waiting for a buffer to become free for filling

template <typename T>
//NOTE: max must be 1 for this
void unit_test::wait_on_fill_threaded(
    std::shared_ptr<recycle_memory<T>> recycler,
    int max,
    T data
) {
// -> check that the last one waits
    bool waiting_unsafe = false;
    std::thread check;
    {
        auto b = recycler->fill();

        std::shared_ptr<std::condition_variable> cv = 
            std::make_shared<std::condition_variable>();
        std::mutex m;

        ASSERT_EQ(b.use_count(), 1) <<
            "Unexpected reference count. Expected 1 and got " +
            std::to_string(b.use_count());

        check = std::thread
            (thread_wait_fill<T>, recycler, cv, std::ref(waiting_unsafe));

        std::unique_lock<std::mutex> lk(m);
        while(!waiting_unsafe) {
            cv->wait(lk);
        }

        *b = data;
        ASSERT_EQ(*b, data) << "Unexpected value. Expected " +
            t2string(data) + " but got " + t2string(*b);

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

template <typename T>
void unit_test::thread_wait_queue(
    std::shared_ptr<recycle_memory<T>> recycler,
    std::shared_ptr<std::condition_variable> cv,
    bool &waiting_unsafe
) {
    waiting_unsafe = true;
    cv->notify_all();
    auto b = recycler->operate();
    EXPECT_EQ(b.use_count(), 1) <<
        "Unexpected reference count. Expected 1 and got " +
        std::to_string(b.use_count());
    waiting_unsafe = false;

}

// exercise waiting for buffer from the operate queue

template <typename T>
void unit_test::buffer_from_empty_queue_threaded(
    std::shared_ptr<recycle_memory<T>> recycler,
    int max,
    T data
) {
// -> check that it waits,
// -> THEN it takes available buffer once queued, check buffer is same

    bool waiting_unsafe = false;
    std::shared_ptr<std::condition_variable> cv = 
        std::make_shared<std::condition_variable>();
    std::mutex m;

    std::thread check(thread_wait_queue<T>, recycler, cv, std::ref(waiting_unsafe));

    std::unique_lock<std::mutex> lk(m);
    while(!waiting_unsafe) {
        cv->wait(lk);
    }

    ASSERT_TRUE(waiting_unsafe) <<
        "Thread did not wait for buffer to be available";

    {
        auto b = recycler->fill();
        *b = data;
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


// exercise thread waiting for buffer from fill where max is more than 1

template <typename T>
//NOTE: max must be >1 for this
void unit_test::wait_multi_take_from_fill_threaded(
    std::shared_ptr<recycle_memory<T>> recycler,
    int max,
    T data
) {
// -> check that the last one waits
    bool waiting_unsafe = false;
    std::shared_ptr<std::condition_variable> cv = 
        std::make_shared<std::condition_variable>();
    std::mutex m;
    std::thread check;

    {
        std::vector<buffer_ptr<T>> buffers;

        // take and hold buffers - they wait to be notified.
        for (int i = 0; i < max; i++) {
            buffers.push_back(recycler->fill());

            EXPECT_EQ(buffers[i].use_count(), 1) <<
            "Unexpected reference count. Expected 1 and got " +
            std::to_string(buffers[i].use_count());

            *buffers[i] = data; 
            ASSERT_EQ(*buffers[i], data) << "Unexpected value. Expected " +
                t2string(data) + " but got " + t2string(*buffers[i]);
        }

        check = std::thread
            (thread_wait_fill<T>, recycler, cv, std::ref(waiting_unsafe));

        std::unique_lock<std::mutex> lk(m);
        while(!waiting_unsafe) {
            cv->wait(lk);
        }

        ASSERT_TRUE(waiting_unsafe) <<
            "Thread did not wait for buffer to be available";

    }

    check.join();

    EXPECT_FALSE(waiting_unsafe) <<
        "Thread did not end and join correctly";

    EXPECT_EQ(recycler->private_free_size(), max) << "Expected " +
        std::to_string(max) + " free buffers. Have " +
        std::to_string(recycler->private_free_size());
}

// exercise reference counting with the buffer & recycle (use a watcher)
// to check if the recycler keeps reference count when stored in a queue

template <typename T>
void unit_test::thread_watcher(
    buffer_ptr<T> p,
    std::shared_ptr<std::condition_variable> cv,
    bool &operating_unsafe,
    int &check_ref
) {
    std::mutex m;
    while(operating_unsafe) {
        std::unique_lock<std::mutex> lk(m);
        cv->wait(lk);

        EXPECT_GE(p.use_count(), check_ref) << "Expected " +
            std::to_string(check_ref) + " free buffers. Have " +
            std::to_string(p.use_count());
    }
}

template <typename T>
void unit_test::watcher_check_reference_counts(
    std::shared_ptr<recycle_memory<T>> recycler
) {
    bool operating_unsafe = true;
    int check_ref = 0;
    std::shared_ptr<std::condition_variable> cv = 
        std::make_shared<std::condition_variable>();
    std::thread watcher;

    {
        auto p = recycler->fill();
        watcher = std::thread(
            thread_watcher<T>, 
            p, 
            cv, 
            std::ref(operating_unsafe), 
            std::ref(check_ref)
        );
        check_ref = 2;
        // notify
        cv->notify_one();

        // now queue
        recycler->queue(p);

        cv->notify_one();
    }

    auto p = recycler->operate();
    cv->notify_one();

    operating_unsafe = false;
    watcher.join();
}

// TODO: exercise multiple writing threads
// TODO: exercise passing by const; 
//       -> check reference count, not able to change buffer

// TODO: exercise destruction of pointer memory (cleanup of recycler) 
//       - might be valgrind??
