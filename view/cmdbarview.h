#ifndef ___CMDBAR_VIEW___H__
#define ___CMDBAR_VIEW___H__

#include "view.h"
#include "../model/cmdbarstate.h"

class CommandBarView : public View {
  CommandBarState *cmdbar;
  bool coloured = false;
    public: 
    CommandBarView(CommandBarState *cmdbar);
  void displayView() override;
};

#endif
