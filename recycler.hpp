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
recycle_memory<T>::recycle_memory(const std::vector<size_t> s, unsigned int max) : shp(s) {
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
    for (auto iter = shp.begin(); iter != shp.end(); iter++) {
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
            memset(reinterpret_cast<void*>(temp), 0xf0, sizeof(T)*size);
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
            delete[] temp;
        }
    }
}

template <typename T>
auto recycle_memory<T>::shape() const noexcept -> const std::vector<size_t>& {
    return shp;
}

template <typename T>
void recycle_memory<T>::return_memory(T* p) {
    std::unique_lock<std::mutex> guard(free_mutex);
    #ifndef NDEBUG
    // buffer_ptr constructor is private so no rogue buffers will ever
    // reach this function
    memset(reinterpret_cast<void*>(p), 0xf0, sizeof(T)*size);
    #endif
    free_q.push_back(p);
    free_variable.notify_one();
}

template <typename T>
auto recycle_memory<T>::free_condition() -> bool {
    return !free_q.empty();
}

template <typename T>
auto recycle_memory<T>::private_free_size() -> int {
    return free_q.size();
}
