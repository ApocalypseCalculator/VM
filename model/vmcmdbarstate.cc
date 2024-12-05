#include <string>
#include "vmcmdbarstate.h"
#include "cursor.h"

VMCommandBarState::VMCommandBarState() {
    content = "";
    searchStr = "";
    searchChar = 0;
}

std::string VMCommandBarState::getCommandBar() {
    return content;
}

void VMCommandBarState::setCommandBar(std::string content) {
    this->content = content;
}

void VMCommandBarState::setCursor(Cursor cursor) {
    this->cursor = cursor;
}

Cursor VMCommandBarState::getCursor() {
    return cursor;
}

void VMCommandBarState::setSearchForward(bool searchForward) {
    this->searchForward = searchForward;
}

void VMCommandBarState::setSearch(std::string searchS) {
    searchStr = searchS;
}

void VMCommandBarState::setSearch(char searchC) {
    searchChar = searchC;
}

std::string VMCommandBarState::getSearchStr() {
    return searchStr;
}

char VMCommandBarState::getSearchChar() {
    return searchChar;
}

bool VMCommandBarState::getSearchForward() {
    return searchForward;
}

void VMCommandBarState::appendCommandBar(std::string content) {
    this->content += content;
}

void VMCommandBarState::appendCommandBar(char c) {
    content += c;
}
