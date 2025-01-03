#ifndef ___CMDBAR_STATE____H_
#define ___CMDBAR_STATE____H_

#include <string>

struct Cursor;

class CommandBarState {
    public: 
    virtual std::string getCommandBar() = 0;
    // set command bar contents, overwrite
    virtual void setCommandBar(std::string content) = 0;
    // append string to command bar
    virtual void appendCommandBar(std::string content) = 0;
    // append char to command bar
    virtual void appendCommandBar(char c) = 0;
    virtual void setPersistMessage(std::string message) = 0;
    virtual std::string getPersistMessage() = 0;
    virtual void setCursor(Cursor cursor) = 0;
    virtual Cursor getCursor() = 0;
    virtual ~CommandBarState() = default;
    // set current search forward status for char search
    virtual void setSearchForward(bool searchForward) = 0;
    // set current search forward status for string search
    virtual void setSearchSForward(bool searchForward) = 0;
    virtual void setSearch(std::string searchS) = 0;
    virtual void setSearch(char searchC) = 0;
    virtual const std::string& getSearchStr() = 0;
    virtual char getSearchChar() = 0;
    // get current search forward status for char search
    virtual bool getSearchForward() = 0;
    // get current search forward status for string search
    virtual bool getSearchSForward() = 0;
    virtual void setError(std::string error) = 0;
    virtual std::string getError() = 0;
    virtual void removeChar() = 0;
};

#endif
