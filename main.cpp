 // main.cpp
#include <vector>
#include <thread>
#include <iostream>
#include <fstream>
#include "r3.hpp"

// ask class for buffers and hand them back
void make_vis(Recycle<int>& r3) {
    // make 5 different integers
    for (int i = 0; i < 5; i++) {
        Reuse<int>* r = r3.new_ptr();
        r->ptr = &i;
        r3.queue_ptr(r);
    }
}

// read from visibility and write to disk (just write to terminal right now)
void read_vis(Recycle<int>& r3, Reuse<int>* vis) {
    //std::ofstream file;
    //file.open("test.txt");
    std::cout << *vis->ptr << std::endl;
    r3.end_operation(vis, NULL); // how do I un-const this? or will this be a completely different function?
    //file.close();
}

// ask for visibility and make thread to read (need to do this continuously??)
void change_vis(Recycle<int>& r3) {
    while (true) { // what is the condition that needs to be here? will this need to be a condition?
        Reuse<int> *vis = r3.start_operation();
        int i = *vis->ptr;
        *vis->ptr = i+1;
        // read_vis should actually be the type of vis's pointer
        Recycle<int>::func f = read_vis;
        std::vector<Recycle<int>::func> v {f};
        r3.end_operation(vis, &v);
    }
}


int main(int argc, char** argv) {
    // make the class
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    Recycle<int> r3 = Recycle<int>(shape, 1); // max number does nothing right now

    // make a thread to be making visibilities
    std::thread making (make_vis, std::ref(r3));

    // make a thread to change visibility
    std::thread changing (change_vis, std::ref(r3));

    making.join();
    changing.join();

    // TODO: how does the main thread not end before all the sub threads from the operating threads end
    std::cout << "end" << std::endl;
}
