#ifndef ___CURSES__KB___H__
#define ___CURSES__KB___H__

#include <vector>
#include "controller.h"
#include "parser.h"
#include "../action/action.h"

class CursesKeyboard : public Controller {
  // virtual Action action() = 0;
  Mode mode;
  VMInputParser parser;
 public:
    CursesKeyboard();
    Action* getAction() override;
    ~CursesKeyboard() override;
    Mode getMode() override;
    void setMode(Mode mode) override;
};
#endif
