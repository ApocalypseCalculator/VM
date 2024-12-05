#ifndef ___VM_STATE____H_
#define ___VM_STATE____H_

#include <memory>
#include <vector>
#include <string>
#include "model.h"
#include "cmdbarstate.h"
#include "filestate.h"

class VMState : public Model {
  std::unique_ptr<CommandBarState> cmdbar;
  std::unique_ptr<FileState> file;
  /*
  Clipboard clipboard;
  EditHistory history;
  Macros macros;
  */
 public: 
    bool fileOwnCursor = true; // does the file currently "own" the cursor
    VMState(const std::string &filename);
    void run() override;
    CommandBarState* getCommandBarState();
    FileState* getFileState();
    ~VMState() override;
};

#endif
