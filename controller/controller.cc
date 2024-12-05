#include <vector>
#include "controller.h"

Controller::~Controller() = default;

const std::vector<int>& Controller::getBuffer() const {
    return buffer;
}

void Controller::flushBuffer() {
    buffer.clear();
}
