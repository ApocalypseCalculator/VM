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
    void save(std::string filename) override;
    bool isReadOnly() override;
    int getLineCount() override;
    int getLineCount(int width) override;
    std::string getLine(int line) override;
    std::string getFilename() override;
    bool hasChange() override;
    void insert(const std::string& s) override;
    void removeLine(int line) override;
    Cursor getCursor() override;
    void setCursor(Cursor newcursor) override;
    void moveCursor(int cols, int lines) override;
    void insertChar(char c) override;
    void removeChar() override;
};

#endif
