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
    setError("");
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

void VMCommandBarState::setSearchSForward(bool searchForward) {
    searchSForward = searchForward;
}

void VMCommandBarState::setSearch(std::string searchS) {
    searchStr = searchS;
}

void VMCommandBarState::setSearch(char searchC) {
    searchChar = searchC;
}

const std::string& VMCommandBarState::getSearchStr() {
    return searchStr;
}

char VMCommandBarState::getSearchChar() {
    return searchChar;
}

bool VMCommandBarState::getSearchForward() {
    return searchForward;
}

bool VMCommandBarState::getSearchSForward() {
    return searchSForward;
}

void VMCommandBarState::appendCommandBar(std::string content) {
    setError("");
    this->content += content;
}

void VMCommandBarState::appendCommandBar(char c) {
    setError("");
    content += c;
}

void VMCommandBarState::setError(std::string error) {
    this->error = error;
}

std::string VMCommandBarState::getError() {
    return error;
}

void VMCommandBarState::removeChar() {
    if(content.size() > 0) {
        content.pop_back();
    }
}

void VMCommandBarState::setPersistMessage(std::string message) {
    setError("");
    content = ""; // reset content
    persistMsg = message;
}

std::string VMCommandBarState::getPersistMessage() {
    return persistMsg;
}
