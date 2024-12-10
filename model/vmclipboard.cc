#include "vmclipboard.h"

void VMClipboard::clear() {
    data = "";
}

const std::string& VMClipboard::get() {
    return data;
}

void VMClipboard::set(std::string s) {
    data = s;
}
