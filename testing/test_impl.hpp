#include <vector>
#include <memory>
#include <thread>
#include <string>
#include <complex>
#include <mutex>
#include <condition_variable>

#include "../lender.hpp"
#include "gtest/gtest.h"
#include "test_class.hpp"

#define UnexpectedEq(check, value, name) EXPECT_EQ(check, value) \
    << "Unexpected " name "."

template <typename T>
void unit_test::null_buffer_ptr() {
    //test create, that it's null
    auto s = buffer_ptr<T>();
    EXPECT_FALSE(s) << "Null buffer pointer is not null";
    
    /* Use Counts cannot be used right now because the shared_ptr compiler 
     * implementation and the stdlib definition tend to disagree with with 
     * the actual counts should be for empty shared_ptrs
     * https://stackoverflow.com/questions/48885252/c-sharedptr-use-count-for-nullptr
     */
    // UnexpectedEq(s.use_count(), 1, "reference count");
}

template <typename T>
void unit_test::buffer_ptr_null_fill(
    std::shared_ptr<library<T>> recycler,
    std::vector<size_t> shape,
    int max
) {
    auto s = buffer_ptr<T>();
    EXPECT_FALSE(s) << "Null buffer pointer is not null";

    s = recycler->fill();
    EXPECT_TRUE(s) << "Buffer pointer still null, expected pointer";
    UnexpectedEq(s.use_count(), 1, "reference count");

    for (int i = 0; i < shape.size(); i++) {
        UnexpectedEq(s->shape[i], shape[i], "shape");
    }

    UnexpectedEq(recycler->private_free_size(),  max - 1, "number of free buffers");
}

template <typename T>
void unit_test::buffer_ptr_fill_null(
    std::shared_ptr<library<T>> recycler,
    std::vector<size_t> shape,
    int max
) {
    auto s = recycler->fill();
    EXPECT_TRUE(s) << "Buffer pointer still null, expected pointer";
    UnexpectedEq(s.use_count(), 1, "reference count");

    for (int i = 0; i < shape.size(); i++) {
        UnexpectedEq(s->shape[i], shape[i], "shape");
    }

    UnexpectedEq(recycler->private_free_size(),  max - 1, "number of free buffers");

    s = nullptr;
    EXPECT_FALSE(s) << "Null buffer pointer is not null";

    UnexpectedEq(recycler->private_free_size(),  max, "number of free buffers");
}

/* single-threaded tests */
// FUTURE: make sure all tests work for arrays as well
// TODO: templatize these tests

// exercise creation of recycler

template <typename T>
void unit_test::make_recycle_memory(std::vector<size_t> shape, int max) {
// -> check correct number of free
    library<T> r(shape, max); // TODO: should this be of base type?

    UnexpectedEq(r.private_free_size(), max, "free list size");
}

// exercise reference counts when taking buffer from fill queue

template <typename T>
void unit_test::take_one_buffer_from_fill(
    std::shared_ptr<library<T>> recycler,
    std::vector<size_t> shape,
    int max
) {
    // -> check shape, number of free (max - 1), num shared_ptr refs
    buffer_ptr<T> p = recycler->fill();

    for (int i = 0; i < shape.size(); i++) {
        UnexpectedEq(p->shape[i], shape[i], "shape");
    }

    UnexpectedEq(recycler->private_free_size(),  max - 1, "number of free buffers");

    UnexpectedEq(p.use_count(), 1, "reference count");

}

// exercise releasing the shared pointer for a buffer and returning memory to
// the recycler

template <typename T>
void unit_test::check_buffer_destruction(
    std::shared_ptr<library<T>> recycler,
    int max
) {
    // -> check number of free is max
    {
        buffer_ptr<T> p = recycler->fill();

        UnexpectedEq(recycler->private_free_size(),  max - 1, "number of free buffers");

        UnexpectedEq(p.use_count(), 1, "reference count");
    }

    // here we assume that destruction of the buffer was done automatically
    // when out of scope

    UnexpectedEq(recycler->private_free_size(),  max, "number of free buffers");

}

// exercise decrementing buffer reference count

template <typename T>
void unit_test::dec_buffer_ref_count(
    std::shared_ptr<library<T>> recycler
) {
    buffer_ptr<T> p = recycler->fill();
    UnexpectedEq(p.use_count(), 1, "reference count");

    recycler->queue(p);
    UnexpectedEq(p.use_count(), 2, "reference count");

    {
        buffer_ptr<T> b = recycler->operate();
        UnexpectedEq(p.use_count(), 2, "reference count");
    }

    UnexpectedEq(p.use_count(), 1, "reference count");
}

// exercise buffers setting data separately from each other

template <typename T>
void unit_test::change_one_buffer(
    std::shared_ptr<library<T>> recycler,
    int max,
    T data
) {
    // -> check new data cannot be accessed from another buffer

    auto b1 = recycler->fill();
    auto b2 = recycler->fill();

    *b1 = data;
    *b2 = data;

    UnexpectedEq(*b1, data, "buffer value");
    UnexpectedEq(*b2, data, "buffer value");

    *b1 = data + data;

    UnexpectedEq(*b1, data+data, "buffer_value");
    UnexpectedEq(*b2, data, "buffer value");

}

// exercise buffers changing values multiple times and it does not interfere 
// with other buffers

template <typename T>
void unit_test::multi_change_buffer(
    std::shared_ptr<library<T>> recycler,
    int max,
    T data,
    T diff
) {
    // -> check new data cannot be accessed from another buffer

    auto b1 = recycler->fill();
    auto b2 = recycler->fill();

    *b1 = data;
    *b2 = data;

    UnexpectedEq(*b1, data, "buffer value");
    UnexpectedEq(*b2, data, "buffer value");

    *b1 = data + diff;

    UnexpectedEq(*b1, data+diff, "buffer value");
    UnexpectedEq(*b2, data, "buffer value");

    *b2 = data - diff;
    
    UnexpectedEq(*b1, data+diff, "buffer value");
    UnexpectedEq(*b2, data-diff, "buffer value");

    *b2 = data;

    UnexpectedEq(*b1, data+diff, "buffer value");
    UnexpectedEq(*b2, data, "buffer value");
    
    *b1 = data;

    UnexpectedEq(*b1, data, "buffer value");
    UnexpectedEq(*b2, data, "buffer value");

}

// exercise setting a buffer (two different ways)

template <typename T>
void unit_test::set_buffer_ptr_array (
    std::shared_ptr<library<T>> recycler
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
        UnexpectedEq(buffer[idx], i, "array value");
    }
}
// UP TO HERE IS DUAL TESTING
// exercise using the operating queue for completed (filled) buffers

template <typename T>
void unit_test::queue_buffer_from_fill (
    std::shared_ptr<library<T>> recycler, 
    int max,
    T data
) {
// -> check number of free, number of queue, number of shared_ptr refs
    auto b = recycler->fill();

    *b = data;
    UnexpectedEq(b.use_count(), 1, "reference count");

    recycler->queue(b);

    UnexpectedEq(recycler->private_free_size(), max - 1, "number of free buffers");
    UnexpectedEq(recycler->private_queue_size(), 1, "queue size");

    UnexpectedEq(b.use_count(), 2, "reference count");
    // one count for this function, one count for the queue

}

//exercise decrement operate queue (max>1 required)

template <typename T>
void unit_test::dec_operate_queue(
    std::shared_ptr<library<T>> recycler
) {
    auto b1 = recycler->fill();
    auto b2 = recycler->fill();
    UnexpectedEq(recycler->private_queue_size(), 0, "queue size");

    recycler->queue(b1);
    UnexpectedEq(recycler->private_queue_size(), 1, "queue size");
    recycler->queue(b2);
    UnexpectedEq(recycler->private_queue_size(), 2, "queue size");

    auto pop1 = recycler->operate();
    UnexpectedEq(recycler->private_queue_size(), 1, "queue size");
    auto pop2 = recycler->operate();
    UnexpectedEq(recycler->private_queue_size(), 0, "queue size");
}


// FUTURE: if user can give own shared_ptrs, check queue has max length

/* Thread safety/ concurrency tests */

template <typename T>
void unit_test::thread_read(buffer_ptr<T> b, T data) {
    EXPECT_GE(b.use_count(), 2) <<
        "Unexpected reference count. Expected at least 2 and got " +
        std::to_string(b.use_count());

    UnexpectedEq(*b, data, "value");

}

// change buffer from main thread and check that same data is read from another

template <typename T>
void unit_test::change_buffer_threaded(
    std::shared_ptr<library<T>> recycler,
    T data
) {
    // -> check new data accessed from same & threads' shared_ptr instance

    auto b = recycler->fill();
    *b = data;
    UnexpectedEq(b.use_count(), 1, "reference count");
    UnexpectedEq(*b, data, "value");

    std::thread check(thread_read<T>, b, data);

    check.join();

    UnexpectedEq(b.use_count(), 1, "reference count");
}

//  change buffer multiple times and check correct data from multiple threads
//  reading same buffer

template <typename T>
void unit_test::multi_change_buffer_threaded(
    std::shared_ptr<library<T>> recycler,
    T data[]
) {
    // -> check new data accessed from same & threads' shared_ptr instance

    auto b = recycler->fill();
    *b = data[0];
    UnexpectedEq(b.use_count(), 1, "reference count");
    UnexpectedEq(*b, data[0], "value");

    std::thread check_one(thread_read<T>, b, data[0]);
    std::thread check_two(thread_read<T>, b, data[0]);

    EXPECT_GE(b.use_count(), 2) <<
        "Unexpected reference count. Expected at least 2 and got " +
        std::to_string(b.use_count());

    check_one.join();
    check_two.join();

    UnexpectedEq(b.use_count(), 1, "reference count");

    *b = data[1];
    UnexpectedEq(*b, data[1], "value");

    std::thread check_three(thread_read<T>, b, data[1]);
    std::thread check_four(thread_read<T>, b, data[1]);

    EXPECT_GE(b.use_count(), 2) <<
        "Unexpected reference count. Expected at least 2 and got " +
        std::to_string(b.use_count());

    check_three.join();
    check_four.join();

    UnexpectedEq(b.use_count(), 1, "reference count");

    *b = data[2];
    UnexpectedEq(*b, data[2], "value");
    
    std::thread check_five(thread_read<T>, b, data[2]);
    std::thread check_six(thread_read<T>, b, data[2]);
    std::thread check_seven(thread_read<T>, b, data[2]);

    EXPECT_GE(b.use_count(), 2) <<
        "Unexpected reference count. Expected at least 2 and got " +
        std::to_string(b.use_count());

    check_five.join();
    check_six.join();
    check_seven.join();

    UnexpectedEq(b.use_count(), 1, "reference count");
}

template <typename T>
void unit_test::thread_wait_fill(
    std::shared_ptr<library<T>> recycler,
    std::shared_ptr<std::condition_variable> cv,
    bool &waiting_unsafe
) {
    waiting_unsafe = true;
    cv->notify_one();
    auto b = recycler->fill();

    UnexpectedEq(b.use_count(), 1, "reference count");

    waiting_unsafe = false;
}

// exercise waiting for a buffer to become free for filling

template <typename T>
//NOTE: max must be 1 for this
void unit_test::wait_on_fill_threaded(
    std::shared_ptr<library<T>> recycler,
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

        UnexpectedEq(b.use_count(), 1, "reference count");

        check = std::thread
            (thread_wait_fill<T>, recycler, cv, std::ref(waiting_unsafe));

        std::unique_lock<std::mutex> lk(m);
        while(!waiting_unsafe) {
            cv->wait(lk);
        }

        *b = data;
        UnexpectedEq(*b, data, "value");

        UnexpectedEq(b.use_count(), 1, "reference count");

        ASSERT_TRUE(waiting_unsafe) <<
            "Thread did not wait for buffer to be available";

        UnexpectedEq(b.use_count(), 1, "reference count");
    }

    check.join();

    EXPECT_FALSE(waiting_unsafe) <<
        "Thread did not end and join correctly";

    UnexpectedEq(recycler->private_free_size(), max, "number of free buffers");
}

template <typename T>
void unit_test::thread_wait_queue(
    std::shared_ptr<library<T>> recycler,
    std::shared_ptr<std::condition_variable> cv,
    bool &waiting_unsafe
) {
    waiting_unsafe = true;
    cv->notify_all();
    auto b = recycler->operate();
    UnexpectedEq(b.use_count(), 1, "reference count");
    waiting_unsafe = false;

}

// exercise waiting for buffer from the operate queue

template <typename T>
void unit_test::buffer_from_empty_queue_threaded(
    std::shared_ptr<library<T>> recycler,
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
        UnexpectedEq(b.use_count(), 1, "reference count");
        recycler->queue(b);
    }

    check.join();

    EXPECT_FALSE(waiting_unsafe) <<
        "Thread did not end and join correctly";

    UnexpectedEq(recycler->private_free_size(), max, "number of free buffers");
}


// exercise thread waiting for buffer from fill where max is more than 1

template <typename T>
//NOTE: max must be >1 for this
void unit_test::wait_multi_take_from_fill_threaded(
    std::shared_ptr<library<T>> recycler,
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

            UnexpectedEq(buffers[i].use_count(), 1, "reference count");

            *buffers[i] = data; 
            UnexpectedEq(*buffers[i], data, "value");
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

    UnexpectedEq(recycler->private_free_size(), max, "number of free buffers");
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
    std::shared_ptr<library<T>> recycler
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

// exercise multiple writing threads

// main thread fills 100x
// m buffers, n threads to read and release buffers

template <typename T>
void unit_test::thread_take_and_release(
    std::shared_ptr<library<T>> recycler
) {
    while(true) { 
        auto p = recycler->operate();
        if (p.kill()) {
            recycler->queue(p.poison_pill());
            break;
        }
        *p = 0;
    }
}

template <typename T>
void unit_test::run_m_threads_n_buffers(
    std::shared_ptr<library<T>> recycler,
    std::vector<T> v,
    int m
) {
    std::vector<std::thread> threads(m); // make m
    for (auto& i : threads) {
        i = std::thread(thread_take_and_release<T>, recycler);
    }
    for(auto i : v) {
        // fill
        auto p = recycler->fill();
        *p = i;
        recycler->queue(p);
    }
    auto p = recycler->fill();
    recycler->queue(p.poison_pill());
    for (auto& i : threads) {
        i.join();
    }
    // take the poison pill off to let the recycler clean everything up properly
    p = recycler->operate();
    ASSERT_TRUE(p.kill());
}


// TODO: exercise passing by const; 
//       -> check reference count, not able to change buffer
