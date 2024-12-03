#ifndef ___VIEW___H__
#define ___VIEW___H__

#include <string>
#include <ncurses.h>

class View{
  WINDOW *win;
  protected: 
  void initview(int h, int w, int y, int x);
 public:
  virtual ~View() = default;
  virtual void update(const std::string& msg) = 0;
  virtual void updateLine(int row, const std::string) = 0;
  virtual void updateCursor(int row, int col) = 0;
  virtual void displayView() = 0;
};

#endif
