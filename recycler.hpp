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

#include "lender.hpp"

template <typename T>
recycle_memory<T>::recycle_memory(const std::vector<size_t> s, unsigned int max) : shape(s) {
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

template <typename T>
recycle_memory<T>::~recycle_memory() {
    /*
     * NOTE: If system is shut down during operation, the memory waiting
     *       in the queue or used by other threads is NOT freed. Unless the 
     *       implementation is changed to allocate memory that cannot be 
     *       cleaned by OS after the process, this should be fine.
     */

    std::lock_guard<std::mutex> guard(free_mutex);

    while (!free_q.empty()) {
        T* temp = free_q.front(); 
        free_q.pop_front();
        if (temp != NULL) {
            delete temp;
        }
    }
}

template <typename T>
auto recycle_memory<T>::fill() -> buffer_ptr<T> {

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

template <typename T>
void recycle_memory<T>::queue(buffer_ptr<T> ptr) {
    std::unique_lock<std::mutex> guard(change_mutex);
    #ifndef NDEBUG
    assert(pointers.find(ptr.get()) != pointers.end());
    #endif
    change_q.push_back(ptr);
    guard.unlock();
    change_variable.notify_one();
}

template <typename T>
auto recycle_memory<T>::operate() -> buffer_ptr<T> {

    std::unique_lock<std::mutex> lock(change_mutex);
    while (change_q.empty()) {
        change_variable.wait(lock);
    }

    buffer_ptr<T> r = change_q.front();
    change_q.pop_front();
    change_mutex.unlock();

    return r;
}

template <typename T>
void recycle_memory<T>::return_memory(T* p) {
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

template <typename T>
auto recycle_memory<T>::change_condition() -> bool {
    return !change_q.empty();
}

template <typename T>
auto recycle_memory<T>::free_condition() -> bool {
    return !free_q.empty();
}

template <typename T>
auto recycle_memory<T>::private_free_size() -> int {
    return free_q.size();
}

template <typename T>
auto recycle_memory<T>::private_queue_size() -> int {
    return change_q.size();
}