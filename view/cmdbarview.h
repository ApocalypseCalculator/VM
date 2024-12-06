#ifndef ___CMDBAR_VIEW___H__
#define ___CMDBAR_VIEW___H__

#include "view.h"
#include "../model/cmdbarstate.h"

class CommandBarView : public View {
  CommandBarState *cmdbar;
  bool coloured = false;
    public: 
    CommandBarView(CommandBarState *cmdbar);
  void update(const std::string& msg) override;
  void updateLine(int row, const std::string) override;
  void updateCursor(int row, int col) override;
  void displayView() override;
};

#endif
