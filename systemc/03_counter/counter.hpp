#pragma once
#include <systemc.h>

/*
N bit counter, so wrap around 2^N

e.g., N=2 -> 0 1 2 3 0 1 2 3 0 1 . . .
*/
template <size_t N> class Counter : public sc_module {
public:
    // Ports
    sc_in<bool> clk;
    sc_in<bool> reset;
    sc_out<sc_uint<N>> count_output;

    SC_CTOR(Counter) : actual_count_{0} {
        SC_METHOD(on_edge);
        sensitive << clk.pos();
        dont_initialize();
    }

private:
    // Internal register for count
    sc_uint<N> actual_count_;

    void on_edge() {
        // If the reset port reads true, reset
        if (reset.read()) {
            actual_count_ = 0;
        }
        else {
            actual_count_++;
        }

        count_output.write(actual_count_);
    }
};
