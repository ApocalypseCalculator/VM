#ifndef ___VIEW___H__
#define ___VIEW___H__

#include <string>
#include <ncurses.h>

class View{
  protected:
  WINDOW *win; 
 public:
  virtual ~View() = default;
  virtual void update(const std::string& msg) = 0;
  virtual void updateLine(int row, const std::string) = 0;
  virtual void updateCursor(int row, int col) = 0;
  virtual void displayView() = 0;
};

#endif
