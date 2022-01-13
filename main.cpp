 // main.cpp
#include <vector>
#include <thread>
#include <iostream>
#include "r3.hpp"

/* ask class for buffers and hand them back */
void make_vis(recycle_memory<int>& r3) {
    //TODO: make multiple buffers test
    buffer_ptr<int> r = r3.fill();
    *r = 5;
    r3.queue(r);
}

/* read from visibility and write to disk (just write to terminal right now) */
void read_vis(buffer_ptr<int> vis) {
    std::cout << "vis:" << *vis << std::endl;
}

/* ask for visibility and make thread to read */
void change_vis(recycle_memory<int>& r3) {
    // what is condition to stop looping this operation and exit gracefully?
    buffer_ptr<int> vis = r3.operate();
    int i = *vis;
    *vis = i+1;
    std::thread reading(read_vis, vis);

    reading.join();
}

void test_int() {
    // make the class
    std::vector<size_t> shape = std::vector<size_t>(1, 1); // n copies of i (n, i)
    int max = 1;
    recycle_memory<int> r3(shape, max); 
    // std::cout << r3.private_free_size() << std::endl;


    // make a thread to be making visibilities
    std::thread making (make_vis, std::ref(r3));

    // make a thread to change visibility
    std::thread changing (change_vis, std::ref(r3));

    making.join();
    changing.join();
    
    // std::cout << r3.private_free_size() << std::endl;
}

void test_array() {
    std::vector<size_t> shape = std::vector<size_t>(2, 2); // n copies of i (n, i)
    int max = 1;
    recycle_memory<int> recycler(shape, max); 
        auto buffer = recycler.fill();

    size_t size = 1;
    for (auto iter = shape.begin(); iter != shape.end(); iter++) {
        size *= *iter;
    }

    // fill each index with its number
    int i = 0;
    for (size_t idx = 0; idx < size; idx++, i++) {
        *(buffer->ptr + idx) = i;

        std::cout << "Expected index " + std::to_string(idx) +
            " to be " + std::to_string(i) + " and got " + 
            std::to_string(buffer[idx]) << std::endl;
    }
}

int main(int argc, char** argv) {
    test_int();
    test_array();

    // TODO: how does the main thread not end before all the sub threads from
    //    the operating threads end
    std::cout << "end" << std::endl;
}
