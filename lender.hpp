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
#include <unordered_map>
#include <condition_variable>

// #define NODEBUG 

/* DESIGN: As template classes, it is assumed that only primitives will be used
 * in the final objects. These are the underlying storage units for every more
 * complicated class used in radio astronomy, so this shall be sufficient for
 * the pointers use in other algorithms.
 */

template <typename T>
class recycle_memory;

template <typename T>
class library;

template <typename T>
class mailbox;

template <typename T>
class buffer_ptr;

struct buffer_deleter {
    template <typename T>
    static auto
    return_to_recycler(recycle_memory<T>& recycle) {
        return [&](T* ptr){ recycle.return_memory(ptr); };
    }
};

/* wrapper object for shared_ptrs */
template <typename T>
class buffer_ptr {
    friend class unit_test;
    friend class mail_test;
    friend class library<T>;
    friend class mailbox<T>;

    private:
        std::shared_ptr<T> sp;
        bool kill_threads;

        buffer_ptr() = default;

        buffer_ptr(T* memory, recycle_memory<T>& recycler);
        /* Give number of shared pointers that have the pointer reference.
         * NOTE: used for testing
         */
        auto use_count() -> int;

    public:
        size_t size;
        // const noexcept are here because shared_ptr had them. tbd on removing

        /* Give reference to the underlying pointer 
         * Will give an int to read from and write to
         */
        auto operator*() const noexcept -> T&;
        /* Give access to an element in the array of the pointer
         * Checks that the index is more than zero
         * Checks that the index is within the size of the array
         */
        auto operator[](int i) const noexcept -> T&;

        operator bool() const noexcept;
        
        buffer_ptr<T>& operator=(std::nullptr_t) noexcept;

        /* Give the raw pointer that is being managed
         * NOTE: the memory itself will still be managed by the recycler, 
         *       meaning that its lifetime is still as expected with other 
         *       buffer_ptrs even though new objects can be made from the raw 
         *       pointer
         */
        auto get() const noexcept -> T*;

        auto poison_pill() -> buffer_ptr<T>;

        auto kill() -> bool;

        void reset();
};

/* recycle_memory class will both MAKE and DESTROY memory that is within the reuseable_buffer class
 *
 * recycle_memory needs to store unused memory for a particular type
 * needs to store queues for threads to take from
 */
template <typename T>
class recycle_memory {
    friend class buffer_deleter;
    friend class buffer_ptr<T>;
    friend class unit_test;

    // has a vector of recycle_memory struct pointers.
    private:
        const std::vector<size_t> shp;
    protected:
        size_t size;
        std::deque<T*> free_q;
        std::mutex free_mutex;
        std::condition_variable free_variable;
        #ifndef NODEBUG
            std::set<T*> pointers;
        #endif

        void return_memory(T* p);

        auto free_condition() -> bool;
        auto private_free_size() -> int;

        /*
         * Instantiator takes in only one type (from the template), 
         * the shape and the max number of buffers for this type. This means
         * recycle_memory will not exceed a certain memory size.
         * 
         * DESIGN: Memory is eagerly allocated in this function to reduce any
         * latency that might appear later in the pipeline for allocation.
         * Users can experiment with optimal # of buffers with variable 'max'
         */

        recycle_memory(const std::vector<size_t> s, unsigned int max);
        /*
         * Destructor must destroy ALL memory that was allocated using their
         * corresponding destructors
         * 
         * DESIGN: Relevant to the pipeline, the memory lender does not provide
         * extra functionality to ensure a clean system shutdown. It is assumed
         * that every thread written can be terminated with malice and will not
         * affect the data output. Specifically, it is assumed that MS files
         * will be written per time integration and that any final grids will 
         * be lost if they are in the course of accumulation.
         * 
         * NOTE: If system is shut down during operation, the memory waiting
         *       in the queue or used by other threads is NOT freed. Unless the 
         *       implementation is changed to allocate memory that cannot be 
         *       cleaned by OS after the process, this should be fine.
         */
        ~recycle_memory();

        auto shape() const noexcept -> const std::vector<size_t>&;

};

template <typename T>
class library : public recycle_memory<T> {
    friend class unit_test;
    private:
        std::deque<buffer_ptr<T>> change_q;
        std::mutex change_mutex;
        std::condition_variable change_variable;
    
        auto change_condition() -> bool;
        auto private_queue_size() -> int;
    public:
        /*
        * Instantiator takes in only one type (from the template), 
        * the shape and the max number of buffers for this type. This means
        * recycle_memory will not exceed a certain memory size.
        * 
        * DESIGN: Memory is eagerly allocated in this function to reduce any
        * latency that might appear later in the pipeline for allocation.
        * Users can experiment with optimal # of buffers with variable 'max'
        */

    library(const std::vector<size_t> s, unsigned int max);

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

template <typename T>
class mailbox : public recycle_memory<T> {
    friend class mail_test;

    private:
        struct map_value {
            std::mutex val_lock;
            std::condition_variable val_cv;
            int read_count;
            buffer_ptr<T> value;

            map_value() {
                read_count = 0;
                value = buffer_ptr<T>();
            }

            map_value(int rcount) {
                read_count = rcount;
                value = NULL;
            }

            map_value(int rcount, buffer_ptr<T> v) {
                read_count = rcount;
                value = v;
            }
        };

        int max_read;
        // Key of time integration, value of custom struct
        std::unordered_map<int, std::shared_ptr<map_value>> box;
        std::mutex box_lock;
        std::condition_variable box_cv;
        #ifndef NODEBUG
            std::set<int> keys;
        #endif

        bool contains_key(int idx);
        bool test_contains_key(int idx);

    public:
        mailbox(const std::vector<size_t> s, int max, int reads = 1);

        auto fill() -> buffer_ptr<T>; // used for writers
        void queue (int key, buffer_ptr<T> ptr);
        auto operate(int key) -> buffer_ptr<T>; // used for readers
};

// TODO: make memory_collection: variadic template for as many buffer types as we want.

#include "buffer.hpp"
#include "recycler.hpp"
#include "library.hpp"
#include "mailbox.hpp"

#endif
