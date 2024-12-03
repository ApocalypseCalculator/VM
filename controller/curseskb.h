#ifndef ___CURSES__KB___H__
#define ___CURSES__KB___H__

#include <vector>
#include "controller.h"
#include "parser.h"

class CursesKeyboard : public Controller {
  // virtual Action action() = 0;
  VMInputParser parser;
 public:
    CursesKeyboard();
    Action* getAction() override;
    ~CursesKeyboard() override;
};
#endif
