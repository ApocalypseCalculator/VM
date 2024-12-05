#ifndef ___VM_FILE_STATE____H_
#define ___VM_FILE_STATE____H_

#include <string>
#include <vector>
#include "filestate.h"
#include "cursor.h"

class VMFileState: public FileState {
    std::vector<std::string> content;
    Cursor cursor;
    bool readonly = false;
    std::string filename;
    bool ghost = false;
    bool changed = false;

    public: 
    VMFileState();
    VMFileState(std::string filename);
    void save() override;
    bool isReadOnly() override;
    int getLineCount() override;
    std::string getLine(int line) override;
    bool hasChange() override;
    void insert(const std::string& s) override;
    void removeLine(int line) override;
    Cursor getCursor() override;
    void setCursor(Cursor newcursor) override;
    void insertChar(char c) override;
};

#endif
