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
 * complicated class used in radio astronomy, so this tends to be sufficient for
 * the pointers used in other algorithms/software.
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

/**
 * @brief smart wrapper for shared_ptr
 * 
 * NOTE: can add in more things like shape
 * 
 */
template <typename T>
class buffer_ptr {
    friend class unit_test;
    friend class mail_test;
    friend class library<T>;
    friend class mailbox<T>;

    private:
        /// contains the ptr to the data; can be empty.
        std::shared_ptr<T> sp;
        bool kill_threads;

        /// default constructor uses the empty constructor from shared_ptr. 
        /// NOTE: has some undefined behavior
        buffer_ptr() = default;

        buffer_ptr(T* memory, recycle_memory<T>& recycler);

        /// NOTE: private function for testing classes
        auto use_count() -> int;

    public:
        /// size of the block NOT in bytes
        size_t size;

        /**
         * @brief Give reference to the underlying pointer 
         * 
         * @return T& the primitive to read from and write to. 
         */
        auto operator*() const noexcept -> T&;

        /**
         * @brief Give access to an element in the array of the pointer
         * 
         * Checks that the index is more than zero
         * Checks that the index is within the size of the array
         * 
         * @param i index of where to read in the array
         * @return T& the primitive to read from and write to. 
         */
        auto operator[](int i) const noexcept -> T&;

        /**
         * @brief check if shared_ptr contains a pointer
         * 
         * @return true data is underlying the shared_ptr
         * @return false no data referenced in the shared_ptr
         */
        operator bool() const noexcept;
        
        /**
         * @brief set the shared_ptr to null/ reset shared_ptr
         * 
         * @return buffer_ptr<T>& the buffer_ptr that has been reset
         */
        buffer_ptr<T>& operator=(std::nullptr_t) noexcept;

        /**
         * @brief Give the raw pointer that is being managed
         * 
         * Made for the data to be used in different software/algorithm.
         * 
         * NOTE: the memory itself will still be managed by the recycler, 
         *       meaning that its lifetime is still as expected with other 
         *       buffer_ptrs even though new objects can be made from the raw 
         *       pointer
         * 
         * @return T* pointer to primitive (array)
         */
        auto get() const noexcept -> T*;

        /**
         * @brief the ending buffer_ptr
         * 
         * Used to indicate to all threads that they may end.
         * 
         * @return buffer_ptr<T> 
         */
        auto poison_pill() -> buffer_ptr<T>;

        /**
         * @brief TODO: I can't remember right now lol
         * 
         * @return true 
         * @return false 
         */
        auto kill() -> bool;

        /**
         * @brief reset shared_ptr
         * 
         * set the shared_ptr to null - uses shared_ptr.reset function
         * 
         */
        void reset();
};

/**
 * @brief base class for memory lenders that manage pointers
 * 
 * recycle_memory class will both MAKE and DESTROY memory that is within the 
 * shared_ptr
 *
 * recycle_memory needs to store unused memory for a particular type
 * needs to store queues for threads to take from
 * 
 * @tparam T primitive (including std::complex)
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
        size_t size; ///< size of the data
        std::deque<T*> free_q; ///< queue of unused pointers for the buffer_ptr
        std::mutex free_mutex; ///< mutex of free pointer queue
        std::condition_variable free_variable; ///< cv of free pointer queue
        #ifndef NODEBUG
            /// test that all pointers in its queues self-originate
            std::set<T*> pointers; 
        #endif

        /**
         * @brief function to reclaim memory
         * 
         * Is given to all created buffer_ptrs so raw pointers boomerang back 
         * to the correct lending object
         * 
         * @param p pointer (of the buffer_ptr that is being destroyed)
         */
        void return_memory(T* p);

        auto free_condition() -> bool;
        auto private_free_size() -> int; ///< testing function

        /**
         * @brief construct the base class (no functions to use pointers)
         *
         * Instantiator takes in only one type (from the template), 
         * the shape and the max number of buffers for this type. This means
         * recycle_memory will not exceed a certain memory size.
         * 
         * DESIGN: Memory is eagerly allocated in this function to reduce any
         * latency that might appear later in the pipeline for allocation.
         * Users can experiment with optimal # of buffers with variable 'max'
         */

        recycle_memory(const std::vector<size_t> s, unsigned int max);

        /**
         * @brief destroy base class, delete raw pointers
         * 
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

        /**
         * @brief TODO: Idk what this is for lol
         * 
         * @return const std::vector<size_t>& 
         */
        auto shape() const noexcept -> const std::vector<size_t>&;

};

/**
 * @brief simple queue-based lender
 * 
 * uses queues to store the buffer pointers for other threads
 * 
 * @tparam T primitive (including std::complex)
 */
template <typename T>
class library : public recycle_memory<T> {
    friend class unit_test;
    private:
        /// queue for data 
        std::deque<buffer_ptr<T>> change_q;
        std::mutex change_mutex; ///< mutex for data queue
        std::condition_variable change_variable; ///< cv for data queue
    
        auto change_condition() -> bool;
        auto private_queue_size() -> int; ///< testing function
    public:
    
        /**
         * @brief construct library class (all usable memory allocated)
         * 
         * Instantiator takes in only one type (from the template), 
         * the shape and the max number of buffers for this type. This means
         * recycle_memory will not exceed a certain memory size.
         * 
         * DESIGN: Memory is eagerly allocated in this function to reduce any
         * latency that might appear later in the pipeline for allocation.
         * Users can experiment with optimal # of buffers with variable 'max'
         */
        library(const std::vector<size_t> s, unsigned int max);

        /**
         * @brief get shared pointer to empty/free buffer
         * 
         * data found in the pointer already is not usable - already been freed
         * 
         * NOTE: this is a blocking operation until a buffer is free
         */
        auto fill() -> buffer_ptr<T>;

        /**
         * @brief give shared pointer back to queue for another thread
         * 
         * this can be used to pass data between threads (although the 
         * buffer_ptr itself can also be used). this is the list of work to be 
         * done
         * 
         * NOTE: this is a blocking operation
         */
        void queue(buffer_ptr<T> ptr);

        /**
         * @brief get a shared pointer from the queue to operate on
         * 
         * NOTE: this is a blocking operation until a buffer is queued
         */
        auto operate() -> buffer_ptr<T>;
};


/**
 * @brief map-based lender
 * 
 * uses a map to store the buffer pointers for other threads. Uses keys so 
 * threads will take the specific data they need.
 * 
 * @tparam T primitive (including std::complex)
 */
template <typename T>
class mailbox : public recycle_memory<T> {
    friend class mail_test;

    private:
        /**
         * @brief value for the map
         * 
         * value is threadsafe, contains the buffer_ptr and the count of 
         * threads that have read it
         */
        struct map_value {
            std::mutex val_lock; ///< mutex for count
            std::condition_variable val_cv; ///< cv for count
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

        int max_read; ///< times that value can be read before being destroyed
        /// Key of time integration, value of custom struct
        std::unordered_map<int, std::shared_ptr<map_value>> box;
        std::mutex box_lock; ///< mutex for map
        std::condition_variable box_cv; ///< mutex for map
        #ifndef NODEBUG
            std::set<int> used_keys;
        #endif

        bool contains_key(int idx);
        bool test_contains_key(int idx);

    public:
        /**
         * @brief construct mailbox class
         * 
         * Instantiator takes in type (from the template), the shape and the max
         * number of buffers for this type, as with recycle_memory and library. 
         * Additionally, it will take in the number of times a buffer can be
         * delivered to another thread in the "mail".
         * 
         * DESIGN: Buffers are eagerly allocated in this function to reduce any
         * latency that might appear later in the pipeline for allocation.
         * Users can experiment with optimal # of buffers with variable 'max'
         * 
         * NOTE: map is dynamically sized, there is no limit to the number of
         *       keys it can contain (even though there is currently a limit
         *       to the number of buffers that the mailbox will keep track of).
         * 
         * TODO: there are cases where buffers may never get the last read 
         * needed to be destroyed OR where a buffer may never arrive for a key.
         * (the second case is assumed to be less likely). In both these cases, 
         * garbage collection is needed to free threads/buffers. A possible 
         * solution for this is a "timeout" in the value. The value
         * could be checked by a thread specific to this purpose, or whenever a 
         * new thread comes in.
         */
        mailbox(const std::vector<size_t> s, int max, int reads = 1);

        /**
         * @brief get shared pointer to empty/free buffer
         * 
         * data found in the pointer already is not usable - already been freed
         * 
         * NOTE: this is a blocking operation until a buffer is free
         */
        auto fill() -> buffer_ptr<T>; ///< used for writers
        
        /**
         * @brief give shared pointer back to queue for another thread
         * 
         * this can be used to deliver a particular buffer (associated with a
         * key) to another thread. this operation can create a key-value pair,
         * and will also fill in an existing pair if it has already been made.
         * 
         * After queuing, the thread will notify all waiting threads.
         * 
         * In debugging mode, there are checks that keys are not used twice.
         * 
         * NOTE: this is a blocking operation
         */
        void queue (int key, buffer_ptr<T> ptr);

        /**
         * @brief get a shared pointer from the map to operate on
         * 
         * this operation can create a key-value pair if none exist. 
         * 
         * DESIGN: The value count will decrement at the beginning before 
         * waiting (if the buffer is not yet delivered). This removes the need 
         * to reaquire both locks if deletion is needed. Because the value is a
         * shared_ptr, it will persist even if the key-value is deleted from the 
         * map.
         * 
         * NOTE: this is a blocking operation until a buffer is added to the key
         */
        auto operate(int key) -> buffer_ptr<T>; ///< used for readers
};

// TODO: make memory_collection: variadic template for as many buffer types as we want.

#include "buffer.hpp"
#include "recycler.hpp"
#include "library.hpp"
#include "mailbox.hpp"

#endif
