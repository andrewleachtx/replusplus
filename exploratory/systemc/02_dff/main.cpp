#include "dff.hpp"

static void tick(sc_signal<bool>& clock) {
    clock.write(false);
    sc_start(5, SC_NS);
    clock.write(true);
    sc_start(5, SC_NS);
}

static void expect(sc_signal<bool>& q, bool expected, const char* msg) {
    bool got = q.read();
    if (got != expected) {
        std::cout << "FAIL @ " << sc_time_stamp() << " : " << msg
                  << " (expected " << expected << ", got " << got << ")\n";
        sc_stop();
    } else {
        std::cout << "PASS @ " << sc_time_stamp() << " : " << msg << "\n";
    }
}

int sc_main(int argc, char* argv[]) {
    sc_signal<bool> clock{"clock"}, d{"d"}, q{"q"};
    
    DFF dut {"device_under_test"};
    dut.clock(clock);
    dut.d(d);
    dut.q(q);

    clock = false;
    d = false;
    sc_start(SC_ZERO_TIME);

    tick(clock);
    expect(q, false, "Q should see a D=0 on posedge");

    // Q shouldn't change unless an edge occurs
    d = true;
    sc_start(2, SC_NS);
    expect(q, false, "Q remains false between edges");

    tick(clock);
    expect(q, true, "Q updates on next posedge");

    return 0;
}
