#ifndef ___VM_STATE____H_
#define ___VM_STATE____H_

#include <memory>
#include <vector>
#include "model.h"

class VMState : public Model {
  /*
  Clipboard clipboard;
  EditHistory history;
  FileState file;
  CommandBarState cmdbar;
  Macros macros;
  */
 public: 
    VMState();
    void run() override;
    ~VMState() override;
};

#endif
