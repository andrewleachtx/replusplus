#pragma once
#include <systemc.h>

/*
    a  b  cin | sum cout
    0  0  0   |  0   0
    0  0  1   |  1   0
    0  1  0   |  1   0
    0  1  1   |  0   1
    1  0  0   |  1   0
    1  0  1   |  0   1
    1  1  0   |  0   1
    1  1  1   |  1   1

    sum <- a ^ b ^ cin
    cout <- (a & b) | (a & cin) | (b & cin)
*/
class Adder : public sc_module {
public:
    sc_in<bool> a;
    sc_in<bool> b;
    sc_in<bool> cin;
    sc_out<bool> sum;
    sc_out<bool> cout;

    // Either input changes to ports can change things (combinational)
    // or by clocks processed by edge changes (sequential)
    SC_CTOR(Adder) {
        // We should only eval every time main updates one of the ports
        SC_METHOD(eval);
        sensitive << a << b << cin;
        dont_initialize();
    }

private:
    void eval() {
        const bool av = a.read();
        const bool bv = b.read();
        const bool cv = cin.read();

        sum.write(av ^ bv ^ cv);
        cout.write((av & bv) | (av & cv) | (bv & cv));
    }
};
