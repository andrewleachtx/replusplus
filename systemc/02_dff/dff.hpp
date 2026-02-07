#pragma once
#include <systemc.h>

class DFF : public sc_module {
public:
    sc_in<bool> clock;
    // d is data_in, q is data_out. q is upside down d is why interestingly
    sc_in<bool> d;
    sc_out<bool> q;

    SC_CTOR(DFF) {
        SC_METHOD(on_posedge);
        sensitive << clock.pos();
    }

private:
    void on_posedge() { q.write(d.read()); }
};
