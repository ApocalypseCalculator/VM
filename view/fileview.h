#ifndef ___FILE_VIEW___H__
#define ___FILE_VIEW___H__

#include "view.h"
#include "../model/filestate.h"

class FileView : public View {
    FileState *file;
    int width;
    int height;
    bool coloured = false;
    // file line offset (not display line offset) for fview window
    int curOffset;

    // update curOffset based on filestate cursor position
    void updateWindow();
    // get the actual display line number for a given file line index
    int getActualDisplayLine(int lineidx, int charidx);

    public: 
    FileView(FileState *file);
  void update(const std::string& msg) override;
  void updateLine(int row, const std::string) override;
  void updateCursor(int row, int col) override;
  void displayView() override;
};

#endif
