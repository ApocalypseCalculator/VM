#include <memory>
#include "parser.h"
#include "../action/action.h"
#include "../action/change.h"
#include "../action/command.h"
#include "../action/move.h"
#include "../action/insert.h"
#include "../action/replace.h"
#include "../action/search.h"
#include "../action/copy.h"
#include "../action/normal.h"
#include "../action/record.h"

VMInputParser::VMInputParser() {
    actions.push_back(std::make_unique<ChangeMode>());
    actions.push_back(std::make_unique<Command>());
    actions.push_back(std::make_unique<MoveCursor>());
    actions.push_back(std::make_unique<Insertion>());
    actions.push_back(std::make_unique<Replace>());
    actions.push_back(std::make_unique<Search>());
    actions.push_back(std::make_unique<CopyPaste>());
    actions.push_back(std::make_unique<Normal>());
    actions.push_back(std::make_unique<RecordReplay>());
}

Action* VMInputParser::parseAction(std::vector<int> input, Mode mode) {
    for(auto &action : actions) {
        if(action->mode == mode && action->matchAction(input)) {
            return action.get();
        }
    }
    return nullptr;
}
