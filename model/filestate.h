#ifndef ___FILE_STATE____H_
#define ___FILE_STATE____H_

#include <string>
#include <vector>
#include "cursor.h"

class FileState {
    public: 
    // save file (:w)
    virtual void save() = 0;
    // save as new file (:w fname)
    virtual void save(std::string fname) = 0;
    virtual bool isReadOnly() = 0;
    virtual const bool isChanged() = 0;
    virtual int getLineCount() = 0;
    virtual int getLineCount(int width) = 0;
    virtual std::string getLine(int line) = 0;
    virtual std::string getFilename() = 0;
    virtual bool hasChange() = 0;
    virtual void insert(const std::string& s) = 0;
    // replace line at line index with new string
    virtual void replaceLine(int line, std::string s) = 0;
    virtual void insertLine(int line, std::string s) = 0;
    virtual void removeLine(int line) = 0;
    virtual Cursor getCursor() = 0;
    virtual void setCursor(Cursor newcursor) = 0;
    virtual void moveCursor(int cols, int lines, bool strict) = 0;
    // insert char at cursor
    virtual void insertChar(char c) = 0;
    // remove char before cursor
    virtual void removeChar() = 0;
    /*get manually defined page movement, 
    represented as half-pages (positive for down)
    default 0 */
    virtual int getForcedPageMove() = 0;
    // set manually defined page movement
    virtual void setForcedPageMove(int offset) = 0;
};

#endif
