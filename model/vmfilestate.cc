#include "vmfilestate.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

VMFileState::VMFileState() {
    cursor = Cursor{0,0};
    filename = "";
    ghost = true;
    content.push_back("");
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
        // don't overwrite file here, open in append mode
        std::ofstream outattempt{filename, std::ios_base::app};
        outattempt.close();
    } catch(...){ // if an error occurs, file is not writable
        readonly = true;
    }
    if(content.empty()) {
        content.push_back("");
    }
}

std::string VMFileState::getFilename() {
    return filename;
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
    changed = false;
}

void VMFileState::save(std::string filename) {
    if(filename != "") {
        return;
    }
    this->filename = filename;
    save();
}

bool VMFileState::isReadOnly() {
    return readonly;
}

int VMFileState::getLineCount() {
    return content.size();
}

int VMFileState::getLineCount(int width) {
    int count = 0;
    for(auto &line : content) {
        count += line.size() / width;
        if(line.size() % width != 0) {
            count++;
        }
    }
    return count;
}

std::string VMFileState::getLine(int line) {
    return content.at(line);
}

bool VMFileState::hasChange() {
    return changed;
}

void VMFileState::insertChar(char c) {
    if(c == '\n') {
        content.insert(content.begin() + cursor.lineidx+1, content.at(cursor.lineidx).substr(cursor.charidx));
        content.at(cursor.lineidx) = content.at(cursor.lineidx).substr(0, cursor.charidx);
        cursor.charidx = 0;
        cursor.lineidx++;
    } else {
        content.at(cursor.lineidx).insert(cursor.charidx, 1, c);
        cursor.charidx++;
    }
    changed = true;
}

void VMFileState::removeChar() {
    if(cursor.charidx == 0) { // merge with previous line
        if(cursor.lineidx == 0) {
            return;
        }
        cursor.charidx = content.at(cursor.lineidx-1).size();
        content.at(cursor.lineidx-1) += content.at(cursor.lineidx);
        content.erase(content.begin() + cursor.lineidx);
        cursor.lineidx--;
    }
    else {
        content.at(cursor.lineidx).erase(cursor.charidx-1, 1);
        cursor.charidx--;
    }
    changed = true;
}

void VMFileState::insert(const std::string& s) {
    for(auto &c : s) {
        insertChar(c);
    }
}

void VMFileState::removeLine(int line) {
    content.at(line).clear();
    changed = true;
}

Cursor VMFileState::getCursor() {
    return cursor;
}

// if cursor is set manually, do not check bounds
void VMFileState::setCursor(Cursor newcursor) {
    cursor = newcursor;
}

void VMFileState::moveCursor(int cols, int lines, bool strict) {
    cursor.lineidx += lines;
    cursor.charidx += cols;
    if(cursor.lineidx < 0) {
        cursor.lineidx = 0;
    }
    if(cursor.lineidx >= content.size()) {
        cursor.lineidx = content.size() - 1;
    }
    if(cursor.charidx < 0) {
        cursor.charidx = 0;
    }
    if(strict) {
        if(cursor.charidx >= content.at(cursor.lineidx).size()) {
            cursor.charidx = content.at(cursor.lineidx).size()-1;
            if(cursor.charidx < 0) {
                cursor.charidx = 0;
            }
        }
    }
    else {
        if(cursor.charidx > content.at(cursor.lineidx).size()) {
            cursor.charidx = content.at(cursor.lineidx).size();
        }
    }
}

int VMFileState::getForcedPageMove() {
    return forcedMove;
}
void VMFileState::setForcedPageMove(int offset) {
    forcedMove = offset;
}
