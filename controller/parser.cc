#include <memory>
#include "parser.h"
#include "../action/action.h"
#include "../action/change.h"
#include "../action/command.h"

VMInputParser::VMInputParser() {
    actions.push_back(std::make_unique<ChangeMode>());
    actions.push_back(std::make_unique<Command>());
}

Action* VMInputParser::parseAction(std::vector<int> input, Mode mode) {
    for(auto &action : actions) {
        if(action->mode == mode && action->matchAction(input)) {
            return action.get();
        }
    }
    return nullptr;
}
