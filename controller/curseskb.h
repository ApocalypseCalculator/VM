#ifndef ___CURSES__KB___H__
#define ___CURSES__KB___H__

#include "controller.h"
#include "parser.h"
#include "../action/action.h"
#include "../model/macros.h"

class CursesKeyboard : public Controller {
  Macros* macro;
  Mode mode;
  VMInputParser parser;
  std::vector<int> textentrybuffer{};

 public:
    CursesKeyboard(Macros* macro);
    Action* getAction() override;
    ~CursesKeyboard() override;
    Mode getMode() override;
    void setMode(Mode mode) override;
    void setReplay(std::string replay) override;
    void setReplay(std::vector<int> replay) override;
    void flushBuffer() override;
    int getMultiplier() override;
    void setMultiplier(int multiplier) override;
    const std::vector<int> getModeBuffer(Mode mode) override;
};
#endif
