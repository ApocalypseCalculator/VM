#ifndef ___VM_STATE____H_
#define ___VM_STATE____H_

#include <memory>
#include <vector>
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
    VMState();
    void run() override;
    CommandBarState* getCommandBarState();
    ~VMState() override;
};

#endif
