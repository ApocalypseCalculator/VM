#ifndef ___VM_STATE____H_
#define ___VM_STATE____H_

#include <memory>
#include <vector>
#include <string>
#include "model.h"
#include "cmdbarstate.h"
#include "filestate.h"
#include "clipboard.h"
#include "macros.h"

class Macros;

class VMState : public Model {
  std::unique_ptr<CommandBarState> cmdbar;
  std::unique_ptr<FileState> file;
  std::unique_ptr<Clipboard> clipboard;
  std::unique_ptr<Macros> macros;
  /*
  EditHistory history;
  */
 public: 
    bool fileOwnCursor = true; // does the file currently "own" the cursor
    VMState(const std::string &filename);
    void run() override;
    CommandBarState* getCommandBarState();
    FileState* getFileState();
    Clipboard* getClipboard();
    Macros* getMacros();
    ~VMState() override;
};

#endif
