#ifndef ___MACROS____H_
#define ___MACROS____H_

#include "vmstate.h"

class Macros {
    public: 
    virtual void record(char key) = 0;
    virtual void stopRecording() = 0;
    virtual bool replay(char key, VMState* vmstate) = 0;
    virtual void append(char key, int inp) = 0;
    virtual char getLast() = 0;
    virtual char isRecording() = 0;
};

#endif
