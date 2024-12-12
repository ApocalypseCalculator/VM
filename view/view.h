#ifndef ___VIEW___H__
#define ___VIEW___H__

#include <string>
#include <ncurses.h>

class View{
  protected:
  WINDOW *win; 
 public:
  virtual ~View() = default;
  virtual void displayView() = 0;
};

#endif
