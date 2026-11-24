#include "counter.hpp"

void tick(sc_signal<bool>& clk) {
    clk.write(false);
    sc_start(5, SC_NS);
    clk.write(true);
    sc_start(5, SC_NS);
}

template <int N>
static void expect(sc_signal<sc_uint<N>>& q, sc_uint<N> expected,
                   const char* msg) {
    sc_uint<N> got = q.read();
    if (got != expected) {
        std::cout << "FAIL @ " << sc_time_stamp() << " : " << msg
                  << " (expected " << expected << ", got " << got << ")\n";
        sc_stop();
    }
    else {
        std::cout << "PASS @ " << sc_time_stamp() << " : " << msg << "\n";
    }
}

int sc_main(int argc, char* argv[]) {
    sc_signal<bool> clk{"clock"}, reset{"reset"};
    sc_signal<sc_uint<3>> count_output;

    Counter<3> dut{"device_under_test"};
    dut.clk(clk);
    dut.reset(reset);
    dut.count_output(count_output);

    clk = false;
    reset = false;
    sc_start(SC_ZERO_TIME);

    tick(clk);
    expect<3>(count_output, 1,
              "One edge change should result in one increment");

    reset = true;
    tick(clk);
    expect<3>(count_output, 0, "After a reset, should be zero");
    tick(clk);
    expect<3>(count_output, 0,
              "While the reset is true, count should stay zero");
    reset = false;

    for (size_t i = 0; i < 7; i++) {
        tick(clk);
    }
    expect<3>(count_output, 7, "One from wrapping");
    tick(clk);
    expect<3>(count_output, 0, "Wrap back to zero");

    return 0;
}
