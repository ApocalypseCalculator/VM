#ifndef ___VM_MACROS____H_
#define ___VM_MACROS____H_

#include <unordered_map>
#include <vector>
#include "macros.h"
#include "vmstate.h"

class VMMacros : public Macros {
    char currentRecording = '\0';
    char lastExecuted = '\0';
    std::unordered_map<char, std::vector<int>> macrokeys;
    public: 
    void record(char key) override;
    void stopRecording() override;
    bool replay(char key, VMState* vmstate) override;
    void append(char key, int inp) override;
    char getLast() override;
    char isRecording() override;
};

#endif
