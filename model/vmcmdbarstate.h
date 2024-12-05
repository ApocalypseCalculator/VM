#ifndef ___VM_CMDBAR_STATE____H_
#define ___VM_CMDBAR_STATE____H_

#include <string>
#include <vector>
#include "cmdbarstate.h"
#include "cursor.h"

class VMCommandBarState : public CommandBarState {
    std::string content;
    bool searchForward = true;
    std::string searchStr;
    char searchChar;
    Cursor cursor;

 public: 
    VMCommandBarState();
    std::string getCommandBar() override;
    void setCommandBar(std::string content) override;
    void setCursor(Cursor cursor) override;
    Cursor getCursor() override;
    void setSearchForward(bool searchForward) override;
    void setSearch(std::string searchS) override;
    void setSearch(char searchC) override;
    std::string getSearchStr() override;
    char getSearchChar() override;
    bool getSearchForward() override;
    void appendCommandBar(std::string content) override;
    void appendCommandBar(char c) override;
};

#endif
