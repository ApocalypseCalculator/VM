#ifndef ___CONTROLLER___H__
#define ___CONTROLLER___H__

#include <string>
class Action;

class Controller {
  // virtual Action action() = 0;
  protected: 
  std::vector<int> buffer;
 public:
  // Action getAction();
  const std::vector<int>& getBuffer() const;
  virtual Action* getAction() = 0;
  virtual ~Controller();
};
#endif
