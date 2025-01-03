#ifndef ___VM_CMDBAR_STATE____H_
#define ___VM_CMDBAR_STATE____H_

#include <string>
#include <vector>
#include "cmdbarstate.h"
#include "cursor.h"

class VMCommandBarState : public CommandBarState {
    std::string content;
    bool searchForward = true;
    bool searchSForward = true;
    std::string searchStr = "";
    char searchChar = '\0';
    Cursor cursor = Cursor{0,-1};
    std::string error;
    std::string persistMsg = "";

 public: 
    VMCommandBarState();
    std::string getCommandBar() override;
    void setCommandBar(std::string content) override;
    void setCursor(Cursor cursor) override;
    Cursor getCursor() override;
    void setSearchForward(bool searchForward) override;
    void setSearchSForward(bool searchForward) override;
    void setSearch(std::string searchS) override;
    void setSearch(char searchC) override;
    const std::string& getSearchStr() override;
    char getSearchChar() override;
    bool getSearchForward() override;
    bool getSearchSForward() override;
    void appendCommandBar(std::string content) override;
    void appendCommandBar(char c) override;
    void setError(std::string error) override;
    std::string getError() override;
    void removeChar() override;
    void setPersistMessage(std::string message) override;
    std::string getPersistMessage() override;
};

#endif
