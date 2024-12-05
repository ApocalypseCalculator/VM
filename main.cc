#include <memory>
#include <string>
#include "model/model.h"
#include "model/vmstate.h"

using namespace std;

int main(int argc, char **argv)
{
    string filename = "";
    if(argc == 2) {
        filename = argv[1];
    }
    unique_ptr<Model> model = make_unique<VMState>(filename);

    while(!model->exit()) {
        model->run();
    }
    return 0;
}
