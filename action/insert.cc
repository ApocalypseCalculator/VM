#include "iostream"
#include "fstream"
#include "insert.h"
#include "../model/vmstate.h"

#define ctrl(x) ((x) & 0x1f)

Insertion::Insertion() {
    mode = Mode::INSERT;
}

void Insertion::doAction(const std::vector<int> &input, VMState *vmstate) {
    // vmstate->insert(input);
    std::ofstream file;
    file.open("test.txt", std::fstream::out);
    file << "Hello World\n"; 
    file.close();
    vmstate->terminate();
}

bool Insertion::matchAction(const std::vector<int> &input) {
    for(auto &val : input) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    if(input.size() > 0 
    && input.at(0) == 'c'
    && input.at(1) == 'b') {
        return true;
    }
    return false;
}

Insertion::~Insertion() = default;
