#ifndef ___FILE_STATE____H_
#define ___FILE_STATE____H_

#include <string>
#include <vector>
#include "cursor.h"

class FileState {
    public: 
    virtual void save() = 0;
    virtual bool isReadOnly() = 0;
    virtual int getLineCount() = 0;
    virtual std::string getLine(int line) = 0;
    virtual bool hasChange() = 0;
    virtual void insert(const std::string& s) = 0;
    virtual void removeLine(int line) = 0;
    virtual Cursor getCursor() = 0;
    virtual void setCursor(Cursor newcursor) = 0;
    virtual void insertChar(char c) = 0;
};

#endif
