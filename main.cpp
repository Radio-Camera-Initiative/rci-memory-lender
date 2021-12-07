 // main.cpp
#include <vector>
#include <thread>
#include <iostream>
#include <fstream>
#include <memory>
#include "r3.hpp"

// ask class for buffers and hand them back
void make_vis(recycle_memory<int>& r3) {
    //TODO: make multiple buffers
    buffer_ptr<int> r = r3.fill();
    *r->ptr = 5;
    r3.queue(r);
}

// read from visibility and write to disk (just write to terminal right now)
void read_vis(buffer_ptr<int> vis) {
    //std::ofstream file;
    //file.open("test.txt");
    std::cout << "vis:" << *vis->ptr << std::endl;
    //file.close();
}

// ask for visibility and make thread to read (need to do this continuously??)
void change_vis(recycle_memory<int>& r3) {
    // what is the condition that needs to be here? will this need to be a condition?
    buffer_ptr<int> vis = r3.operate();
    int i = *vis->ptr;
    *vis->ptr = i+1;
    std::thread reading(read_vis, vis);

    reading.join();
}

void test_int() {
    // make the class
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    recycle_memory<int> r3 = recycle_memory<int>(shape, 1); 
    std::cout << r3._free_size() << std::endl;


    // make a thread to be making visibilities
    std::thread making (make_vis, std::ref(r3));

    // make a thread to change visibility
    std::thread changing (change_vis, std::ref(r3));

    making.join();
    changing.join();
    
    std::cout << r3._free_size() << std::endl;
}

int main(int argc, char** argv) {
    test_int();

    // TODO: how does the main thread not end before all the sub threads from the operating threads end
    std::cout << "end" << std::endl;
}
