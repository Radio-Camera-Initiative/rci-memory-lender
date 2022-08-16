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
buffer_ptr<T>::buffer_ptr(T* memory, recycle_memory<T>& recycler) {
    sp = std::shared_ptr<T>(memory, buffer_deleter::return_to_recycler(recycler));
    size = recycler.size;
    kill_threads = false;
}

template <typename T>
auto buffer_ptr<T>::use_count() -> int {
    return sp.use_count();
}

template <typename T>
auto buffer_ptr<T>::operator*() const noexcept -> T& {
    return *sp.get();
}

template <typename T>
auto buffer_ptr<T>::operator[](int i) const noexcept -> T&{
    assert(i >= 0);
    assert(static_cast<size_t>(i) < size);
    return sp.get()[i];
}

template <typename T>
buffer_ptr<T>::operator bool() const noexcept{
    return sp ? true : false;
}

/* The buffer_ptr doesn't necessarily hold a nullptr,
 * it is just setting shared_ptr to empty (using the reset method)
 *
 * Side Note: As far as I can find
 * there is no real way for a shared_ptr to hold a null_ptr properly anyway,
 * they just tend to be empty.
 */
template <typename T>
buffer_ptr<T>& buffer_ptr<T>::operator=(std::nullptr_t) noexcept {
    this->reset();
    return *this;
}

template <typename T>
auto buffer_ptr<T>::get() const noexcept -> T* {
    return sp.get();
}

template <typename T>
auto buffer_ptr<T>::poison_pill() -> buffer_ptr<T>{
    kill_threads = true;
    return *this;
}

template <typename T>
auto buffer_ptr<T>::kill() -> bool {
    return kill_threads;
}

template <typename T>
void buffer_ptr<T>::reset() {
    sp.reset();
    size = 0;
    kill_threads = false;
}
