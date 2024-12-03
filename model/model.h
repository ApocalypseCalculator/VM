#ifndef ___MODEL____H_
#define ___MODEL____H_

#include <memory>
#include <vector>
#include "../controller/controller.h"
//#include "action.h"
#include "../view/view.h"

class Model{
  bool exitFlag;
 protected:
  Model();
  std::vector<std::unique_ptr<View>> views;
  std::unique_ptr<Controller> control;
  void addView(std::unique_ptr<View> v);
  void setController(std::unique_ptr<Controller> v);
  //Action getAction();
 public:
  virtual void run() = 0;
  virtual ~Model() = default;
  // void updateViews(int, int, State);
  void updateViews(const std::string& s);
  void displayViews();
  void terminate();
  bool exit();
};

#endif
