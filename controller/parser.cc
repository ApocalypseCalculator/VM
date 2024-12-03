#include <memory>
#include "parser.h"
#include "../action/action.h"
#include "../action/insert.h"

VMInputParser::VMInputParser() {
    mode = Mode::NORMAL;
    actions.push_back(std::make_unique<Insertion>());
}

Action* VMInputParser::parseAction(std::vector<int> input) {
    for(auto &action : actions) {
        if(action->mode == mode && action->matchAction(input)) {
            return action.get();
        }
    }
    return nullptr;
}
