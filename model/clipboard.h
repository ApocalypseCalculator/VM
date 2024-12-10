#ifndef ___CLIPBOARD____H_
#define ___CLIPBOARD____H_

#include <string>

class Clipboard {
    public: 
    virtual void clear() = 0;
    virtual const std::string& get() = 0;
    virtual void set(std::string s) = 0;
};

#endif
