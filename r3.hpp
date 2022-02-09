#ifndef R3_H
#define R3_H

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

        reuseable_buffer(T* p, recycle_memory<T>& r) : recycle(r) {
            shape = recycle.shape;
            ptr = p;
        }

        ~reuseable_buffer() {
            recycle.return_memory(ptr);
        }

        T& operator[](unsigned int i) const noexcept {
            // TODO: what to check here when indexing?
            return *(ptr + i);
        }
};

/* wrapper object for shared_ptrs */
template <typename T>
class buffer_ptr {
    friend class unit_test;
    friend class recycle_memory<T>;

    private:
        std::shared_ptr<reuseable_buffer<T>> sp;
        size_t size;

        buffer_ptr(T* memory, recycle_memory<T>& recycler) {
            sp = std::make_shared<reuseable_buffer<T>>(memory, recycler);
            size = recycler.size;
        }

        int use_count() {
            return sp.use_count();
        }

    public:

        // const noexcept are here because shared_ptr had them. tbd on removing
        T& operator*() const noexcept {
            return *(sp->ptr);
        }

        reuseable_buffer<T>* operator->() const noexcept {
            return sp.get();
        }

        T& operator[](int i) const noexcept {
            assert(i >= 0);
            assert(static_cast<size_t>(i) < size);
            return *(sp->ptr + i);
        }

        T* get() const noexcept {
            return sp->ptr;
        }
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

        void return_memory(T* p) {
            std::unique_lock<std::mutex> guard(free_mutex);
            #ifndef NDEBUG
            // buffer_ptr constructor is private so no rogue buffers will ever 
            // reach this function
            memset(p, 0xf0, sizeof(T)*size);
            #endif
            free_q.push_back(p);
            guard.unlock();
            free_variable.notify_one();
        }

        bool change_condition() {
            return !change_q.empty();
        }

        bool free_condition() {
            return !free_q.empty();
        }

        int private_free_size() {
            return free_q.size();
        }

        int private_queue_size() {
            return change_q.size();
        }

    public:
        /*
         * Instantiator takes in only one type (from the template), 
         * the shape and the max number of buffers for this type. This means
         * recycle_memory will not exceed a certain memory size.
         */
        recycle_memory(std::vector<size_t> s, unsigned int max) : shape(s) {
            change_q = std::deque<buffer_ptr<T>>();
            free_q = std::deque<T*>();
            #ifndef NDEBUG
            pointers = std::set<T*>();
            #endif

            // No other thread should interfere in the constructor, but this 
            // is to prevent any instruction reordering
            std::lock_guard<std::mutex> guard(free_mutex);

            // Centralize allocation avoid waiting later on. Assumes all 
            // memory is used
            size = 1;
            for (auto iter = shape.begin(); iter != shape.end(); iter++) {
                size *= *iter;
            }

            for (unsigned int i = 0; i < max; i++) {
                // use nothrow because we don't do anything with the exception
                T* temp = new(std::nothrow) T[size];
                // TODO: for testing this should be set to a known value
                if (temp == nullptr) {
                    // we could set a different value as max in the object
                    break;
                }

                #ifndef NDEBUG
                memset(temp, 0xf0, sizeof(T)*size);
                pointers.insert(temp);
                #endif
                free_q.push_back(temp);
            }
        }
        /*
         * Destructor must destroy ALL memory that was allocated using their
         * corresponding destructors
         */
        ~recycle_memory() {
            // TODO: make sure all buffers have released their memory to free

            std::lock_guard<std::mutex> guard(free_mutex);

            while (!free_q.empty()) {
                T* temp = free_q.front(); 
                free_q.pop_front();
                if (temp != NULL) {
                    delete temp;
                }
            }
        }

        /* get a shared pointer to the buffer we want to fill with data 
         * NOTE: this is a blocking operation until a buffer is free
         */
        buffer_ptr<T> fill() {

            std::unique_lock<std::mutex> lock(free_mutex);
            while(free_q.empty()) {  
                free_variable.wait(lock);
            }

            // take from free list
            T* ptr = free_q.front();
            free_q.pop_front();

            #ifndef NDEBUG
            // check there was no changes after free
            T* f = new T();
            memset(f, 0xf0, sizeof(T));
            assert(memcmp(ptr, f, sizeof(T)) == 0);
            memset(ptr, 0, sizeof(T)*size);
            #endif

            // make reuseable_buffer for the buffer
            auto sp = buffer_ptr<T>(ptr, *this);
            return sp;

        }

        /* give a shared pointer back to be queued for operation 
         * NOTE: this is a blocking operation
         */
        void queue(buffer_ptr<T> ptr) {
            std::unique_lock<std::mutex> guard(change_mutex);
            #ifndef NDEBUG
            assert(pointers.find(ptr.get()) != pointers.end());
            #endif
            change_q.push_back(ptr);
            guard.unlock();
            change_variable.notify_one();
        }

        /* get a shared pointer from the queue to operate on  
         * NOTE: this is a blocking operation until a buffer is queued
         */
        buffer_ptr<T> operate() {

            std::unique_lock<std::mutex> lock(change_mutex);
            while (change_q.empty()) {
                change_variable.wait(lock);
            }

            buffer_ptr<T> r = change_q.front();
            change_q.pop_front();
            change_mutex.unlock();

            return r;
        }
};

// TODO: make memory_collection: variadic template for as many buffer types as we want.

#endif
