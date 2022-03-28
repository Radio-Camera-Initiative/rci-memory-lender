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
reuseable_buffer<T>::reuseable_buffer(T* p, recycle_memory<T>& r) : recycle(r) {
    shape = recycle.shape;
    ptr = p;
}

template <typename T>
reuseable_buffer<T>::~reuseable_buffer() {
    recycle.return_memory(ptr);
}

template <typename T>
auto reuseable_buffer<T>::operator[](unsigned int i) const noexcept -> T& {
    // TODO: what to check here when indexing?
    return *(ptr + i);
}


template <typename T>
buffer_ptr<T>::buffer_ptr(T* memory, recycle_memory<T>& recycler) {
    sp = std::make_shared<reuseable_buffer<T>>(memory, recycler);
    size = recycler.size;
}

template <typename T>
auto buffer_ptr<T>::use_count() -> int {
    return sp.use_count();
}

template <typename T>
auto buffer_ptr<T>::operator*() const noexcept -> T& {
    return *(sp->ptr);
}

template <typename T>
auto buffer_ptr<T>::operator->() const noexcept -> reuseable_buffer<T>* {
    return sp.get();
}

template <typename T>
auto buffer_ptr<T>::operator[](int i) const noexcept -> T&{
    assert(i >= 0);
    assert(static_cast<size_t>(i) < size);
    return *(sp->ptr + i);
}

template <typename T>
auto buffer_ptr<T>::get() const noexcept -> T* {
    return sp->ptr;
}