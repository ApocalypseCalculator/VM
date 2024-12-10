#ifndef ___VMCLIPBOARD____H_
#define ___VMCLIPBOARD____H_

#include <string>
#include "clipboard.h"

class VMClipboard : public Clipboard {
    std::string data = "";
    public: 
    void clear() override;
    const std::string& get() override;
    void set(std::string s) override;
};

#endif
