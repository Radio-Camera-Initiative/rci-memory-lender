#ifndef LENDER_H
#define LENDER_H

#include <vector>
#include <queue>
#include <memory>
#include <new>
#include <set>
#include <mutex>
#include <cstring>
#include <cassert>
#include <algorithm>
#include <condition_variable>

// #define NDEBUG 

template <typename T>
class recycle_memory;

template <typename T>
class buffer_ptr;

template <typename T>
class reuseable_buffer {
    friend class buffer_ptr<T>;
    friend class unit_test;
    
    private:
        recycle_memory<T>& recycle;
        // if this was shared_ptr, would memory be deleted at destruction?
        T* ptr; 

    public: 
        std::vector<size_t> shape;

        reuseable_buffer(T* p, recycle_memory<T>& r);
        ~reuseable_buffer();
        auto operator[](unsigned int i) const noexcept -> T&;
};

/* wrapper object for shared_ptrs */
template <typename T>
class buffer_ptr {
    friend class unit_test;
    friend class recycle_memory<T>;

    private:
        std::shared_ptr<reuseable_buffer<T>> sp;
        size_t size;

        buffer_ptr(T* memory, recycle_memory<T>& recycler);
        /* Give number of shared pointers that have the pointer reference.
         * NOTE: used for testing
         */
        auto use_count() -> int;

    public:
        // const noexcept are here because shared_ptr had them. tbd on removing

        /* Give reference to the underlying pointer 
         * Will give an int to read from and write to
         */
        auto operator*() const noexcept -> T&;
        /* Give access to any public functions/elements in reuseable_buffer<T> 
         */
        auto operator->() const noexcept -> reuseable_buffer<T>*;
        /* Give access to an element in the array of the pointer
         * Checks that the index is more than zero
         * Checks that the index is within the size of the array
         */
        auto operator[](int i) const noexcept -> T&;
        /* Give the raw pointer that is being managed
         * NOTE: the memory itself will still be managed by the recycler, 
         *       meaning that its lifetime is still as expected with other 
         *       buffer_ptrs even though new objects can be made from the raw 
         *       pointer
         */
        auto get() const noexcept -> T*;
};

/* recycle_memory class will both MAKE and DESTROY memory that is within the reuseable_buffer class
 *
 * recycle_memory needs to store unused memory for a particular type
 * needs to store queues for threads to take from
 */
template <typename T>
class recycle_memory {
    friend struct reuseable_buffer<T>;
    friend class buffer_ptr<T>;
    friend class unit_test;

    // has a vector of recycle_memory struct pointers.
    private:
        const std::vector<size_t> shape;
        size_t size;
        std::deque<buffer_ptr<T>> change_q;
        std::mutex change_mutex;
        std::condition_variable change_variable;
        std::deque<T*> free_q;
        std::mutex free_mutex;
        std::condition_variable free_variable;
        #ifndef NDEBUG
        std::set<T*> pointers;
        #endif

        void return_memory(T* p);

        auto change_condition() -> bool;
        auto free_condition() -> bool;
        auto private_free_size() -> int;
        auto private_queue_size() -> int;

    public:
        /*
         * Instantiator takes in only one type (from the template), 
         * the shape and the max number of buffers for this type. This means
         * recycle_memory will not exceed a certain memory size.
         */

        recycle_memory(const std::vector<size_t> s, unsigned int max);
        /*
         * Destructor must destroy ALL memory that was allocated using their
         * corresponding destructors
         */
        ~recycle_memory();

        /* get a shared pointer to the buffer we want to fill with data 
         * NOTE: this is a blocking operation until a buffer is free
         */
        auto fill() -> buffer_ptr<T>;

        /* give a shared pointer back to be queued for operation 
         * NOTE: this is a blocking operation
         */
        void queue(buffer_ptr<T> ptr);

        /* get a shared pointer from the queue to operate on  
         * NOTE: this is a blocking operation until a buffer is queued
         */
        auto operate() -> buffer_ptr<T>;
};

// TODO: make memory_collection: variadic template for as many buffer types as we want.

#include "buffer.hpp"
#include "recycler.hpp"

#endif
