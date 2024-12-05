#include "model.h"
#include "../controller/controller.h"

Model::Model() {
    exitFlag = false;
}

void Model::addView(std::unique_ptr<View> v) {
    views.push_back(std::move(v));
}

void Model::setController(std::unique_ptr<Controller> v) {
    control = std::move(v);
}

void Model::terminate() {
    exitFlag = true;
}

bool Model::exit() {
    return exitFlag;
}

Controller* Model::getController() {
    return control.get();
}
