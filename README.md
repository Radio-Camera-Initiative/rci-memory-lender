# r3_central_manager

Requirements for use:

The Recycler (`recycle_memory`) must outlive all objects and threads, to be the last destroyed object.

The `shared_ptr`s to `reuseable_buffer`s (`buffer_ptr`s) must be given to each thread by copy constructor, not by reference to extend their lifetime. Threads that only read the pointer data are included in this. However, it is required that only one thread be changing the data in the `buffer_ptr` at one time, as it is not threadsafe.

### Testing

Testing is done using GoogleTest and built using CMake. From the main directory, follow these commands:
1. `cd testing`
2. `cmake -S . -B build`
3. `cmake --build build`
This will build all current tests. To run those tests, move into the `build` directory and run `ctest`.