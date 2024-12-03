#ifndef ___FILE_STATE____H_
#define ___FILE_STATE____H_

#include <string>
#include <vector>

class FileState {
    protected: 
    std::string name;
    bool readOnly;

    public: 
    virtual void save() = 0;
    virtual bool isReadOnly() = 0;
    virtual int getLineCount() = 0;
    virtual std::string getLine(int line) = 0;
};

#endif
