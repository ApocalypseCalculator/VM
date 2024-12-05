#ifndef ___CONTROLLER___H__
#define ___CONTROLLER___H__

#include <string>
#include "../action/action.h"
class Action;

class Controller {
  // virtual Action action() = 0;
  protected: 
  std::vector<int> buffer;
 public:
  // Action getAction();
  const std::vector<int>& getBuffer() const;
  void flushBuffer();
  virtual Action* getAction() = 0;
  virtual Mode getMode() = 0;
  virtual void setMode(Mode mode) = 0;
  virtual ~Controller();
};
#endif
