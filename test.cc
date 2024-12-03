
#include "model/model.h"
#include "model/vmstate.h"
#include <ncurses.h>

using namespace std;

int main(int argc, char **argv)
{
    unique_ptr<Model> model = make_unique<VMState>();

    while(!model->exit()) {
        model->run();
    }
    return 0;
}
