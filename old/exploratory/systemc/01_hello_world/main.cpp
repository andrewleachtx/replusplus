#include <systemc.h>
using namespace sc_core;

class HELLO_WORLD_MODULE : public sc_module {
public:
    SC_CTOR(HELLO_WORLD_MODULE) {
        printf("%s constructor\n", name());
    }
};

int sc_main(int argc, char* argv[]) {
    HELLO_WORLD_MODULE module_a("module_a");
    sc_start();

    return 0;
}
