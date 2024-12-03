#ifndef ___VM_INPUT_PARSER___H__
#define ___VM_INPUT_PARSER___H__

#include <vector>
#include <memory>
#include "controller.h"
#include "../action/action.h"

class VMInputParser{
    Mode mode;
    std::vector<std::unique_ptr<Action>> actions;
  // virtual Action action() = 0;
 public:
    VMInputParser();
    Action* parseAction(std::vector<int> input);
  // Action getAction();
};
#endif
