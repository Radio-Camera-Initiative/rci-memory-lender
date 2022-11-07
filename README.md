# RCI Memory Lender

[Documentation](https://radio-camera-initiative.github.io/rci-memory-lender/html/index.html)

Requirements for use:

The Recycler (`recycle_memory`) must outlive all objects and threads, to be the last destroyed object.

The `shared_ptr` of a `reuseable_buffer` (name `buffer_ptr`) must be given to each thread by copy constructor, not by reference, to extend the lifetime of the memory. Threads that only read the pointer data are included in this. However, it is required that only one thread be changing the data in the `buffer_ptr` at one time, as it is not threadsafe.

When a `buffer_ptr` stops being used (specifically, when all references go out of scope), the system will give the memory that the pointer had a reference to back to the recycler, such that the memory can be used again for the same data type.

### Testing

Testing is done using GoogleTest and built using CMake. From the main directory, follow these commands:
1. `cd testing`
2. `cmake -S . -B build`
3. `cmake --build build`
This will build all current tests. To run those tests, move into the `build` directory and run `ctest`.

### Pointer Life Cycle

The `buffer_ptr` object allows the user to `get` the raw pointer to memory. This is a necessary function for use of that memory in different softwares, but be aware of the danger to go along with this.

**The memory pointed to by the raw pointer is still also held in the buffer pointer**

The danger in this is as follows: if `buffer_ptr` which the user gets the raw pointer from goes out of scope (from the function, the thread, etc), then the memory that is pointed to is in danger of being reaped and reused. In these circumstances, the program using said memory will not know whether the memory is safe. It is the users' responsibility to ensure that the memory is still safe for the function to use, throughout its life cycle. 

The following are things to keep in mind:
* Even if the user takes a raw pointer from `buffer_ptr`, the memory is still not threadsafe. Only one thread should be changing the data at a time.
* Any software use is suggested to be synchronous in threads rather than asynchronous. This makes keeping the `buffer_ptr` in scope more likely.
* Users should *never* delete a `buffer_ptr`. If a specific scope is needed, use `{}` around said scope.