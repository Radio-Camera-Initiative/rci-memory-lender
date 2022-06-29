#include <vector>
#include <memory>
#include <thread>
#include <string>
#include <complex>
#include <mutex>
#include <condition_variable>

#include "../lender.hpp"
#include "gtest/gtest.h"
#include "test_mail_class.hpp"

#define UnexpectedEq(check, value, name) EXPECT_EQ(check, value) \
    << "Unexpected " name "."

template <typename T>
void mail_test::make_mailbox(std::vector<size_t> shape, int max) {
// -> check correct number of free
    mailbox<T> r(shape, max);

    UnexpectedEq(r.private_free_size(), max, "free list size");
}

// exercise reference counts when taking buffer from fill queue

template <typename T>
void mail_test::take_one_buffer_from_fill(
    std::shared_ptr<mailbox<T>> recycler,
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
void mail_test::check_buffer_destruction(
    std::shared_ptr<mailbox<T>> recycler,
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
// TODO: more mailbox/recycler agnostic way of testing this.
// TOTHINK: is testing the pointer in this way again necessary if both objects 
//          using same buffer_ptr?
template <typename T>
void mail_test::dec_buffer_ref_count(
    std::shared_ptr<mailbox<T>> recycler
) {
    buffer_ptr<T> p = recycler->fill();
    UnexpectedEq(p.use_count(), 1, "reference count");

    recycler->queue(10, p);
    UnexpectedEq(p.use_count(), 2, "reference count");

    {
        buffer_ptr<T> b = recycler->operate(10);
        UnexpectedEq(p.use_count(), 2, "reference count");
    }

    UnexpectedEq(p.use_count(), 1, "reference count");
}

// exercise buffers setting data separately from each other

template <typename T>
void mail_test::change_one_buffer(
    std::shared_ptr<mailbox<T>> recycler,
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
void mail_test::multi_change_buffer(
    std::shared_ptr<mailbox<T>> recycler,
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
void mail_test::set_buffer_ptr_array (
    std::shared_ptr<mailbox<T>> recycler
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