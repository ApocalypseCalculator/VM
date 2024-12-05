#include "vmfilestate.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

VMFileState::VMFileState() {
    cursor = Cursor{0,0};
    filename = "";
    ghost = true;
}

VMFileState::VMFileState(std::string filename) {
    this->filename = filename;
    cursor = Cursor{0,0};
    std::ifstream file;
    try {
        file = std::ifstream{filename};
    } catch (...) { // if an error occurs, file is not readable
        // todo: throw a more specific exception
        throw;
    }
    std::string lineval;
    while(getline(file, lineval)) {
        content.push_back(lineval);
    }
    file.close();
    try {
        std::ofstream outattempt{filename};
    } catch(...){ // if an error occurs, file is not writable
        readonly = true;
    }
}

void VMFileState::save() {
    if(readonly) {
        return;
    }
    std::ofstream out;
    try {
        out = std::ofstream{filename};
    }
    catch(...) { // some writing error
        throw;
    }
    for(auto &line : content) {
        out << line << std::endl;
    }
    out.close();
}

bool VMFileState::isReadOnly() {
    return readonly;
}

int VMFileState::getLineCount() {
    return content.size();
}

std::string VMFileState::getLine(int line) {
    return content.at(line);
}

bool VMFileState::hasChange() {
    return changed;
}

void VMFileState::insertChar(char c) {
    content.at(cursor.lineidx).insert(cursor.charidx, 1, c);
    cursor.charidx++;
    if(c == '\n') {
        cursor.charidx = 0;
        content.insert(content.begin() + cursor.lineidx, "");
        cursor.lineidx++;
    }
    changed = true;
}

void VMFileState::insert(const std::string& s) {
    for(auto &c : s) {
        insertChar(c);
    }
}

void VMFileState::removeLine(int line) {
    content.erase(content.begin() + line);
    changed = true;
}

Cursor VMFileState::getCursor() {
    return cursor;
}

void VMFileState::setCursor(Cursor newcursor) {
    cursor = newcursor;
}
