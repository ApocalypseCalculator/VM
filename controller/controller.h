#ifndef ___CONTROLLER___H__
#define ___CONTROLLER___H__

#include <string>
#include <vector>
#include <stack>
#include "../action/action.h"
class Action;

class Controller {
  // virtual Action action() = 0;
  protected: 
  std::vector<int> buffer;
  std::stack<int> replay;
  int multiplier = 0;
 public:
  // Action getAction();
  const std::vector<int>& getBuffer() const;
  // this is intended as a patch to allow history to access 
  // a stale buffer for redo
  virtual const std::vector<int> getModeBuffer(Mode mode) = 0;
  virtual void flushBuffer() = 0;
  virtual Action* getAction() = 0;
  virtual Mode getMode() = 0;
  virtual void setMode(Mode mode) = 0;
  virtual void setReplay(std::string replay) = 0;
  virtual void setReplay(std::vector<int> replay) = 0;
  virtual int getMultiplier() = 0;
  virtual void setMultiplier(int multiplier) = 0;
  virtual ~Controller();
};
#endif
