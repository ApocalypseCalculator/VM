#include "vmmacros.h"

void VMMacros::record(char key) {
    char lcase = std::tolower(key);
    if(lcase != key) { // key is uppercase, append mode
        if(!macrokeys.contains(lcase)) {
            macrokeys.insert({lcase, std::vector<int>()});
        }
    }
    else { // key is lowercase, overwrite
        if(!macrokeys.contains(lcase)) {
            macrokeys.insert({lcase, std::vector<int>()});
        }
        else {
            macrokeys.at(lcase).clear();
        }
    }
    currentRecording = lcase;
}

bool VMMacros::replay(char key, VMState* vmstate) {
    if(macrokeys.contains(key)) {
        vmstate->getController()->setReplay(macrokeys.at(key));
        lastExecuted = key;
        return true;
    }
    else {
        return false;
    }
}

void VMMacros::append(char key, int inp) {
    if(macrokeys.contains(key)) {
        macrokeys.at(key).push_back(inp);
    }
}

void VMMacros::stopRecording() {
    // remove trailing q if it exists
    if(macrokeys.contains(currentRecording) && !macrokeys.at(currentRecording).empty() && macrokeys.at(currentRecording).back() == 'q') {
        macrokeys.at(currentRecording).pop_back();
    }
    currentRecording = '\0';
}

char VMMacros::isRecording() {
    return currentRecording;
}

char VMMacros::getLast() {
    return lastExecuted;
}
