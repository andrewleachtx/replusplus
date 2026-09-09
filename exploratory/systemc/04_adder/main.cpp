#include "adder.hpp"
#include <sstream>

static void expect(const char* name, bool got_sum, bool got_cout, bool exp_sum,
                   bool exp_cout, bool a, bool b, bool cin) {
    if (got_sum != exp_sum || got_cout != exp_cout) {
        std::ostringstream oss;
        oss << name << " FAIL @ " << sc_time_stamp() << "  a=" << a
            << " b=" << b << " cin=" << cin << "  got(sum=" << got_sum
            << ", cout=" << got_cout << ")"
            << "  exp(sum=" << exp_sum << ", cout=" << exp_cout << ")";
        SC_REPORT_ERROR("TB", oss.str().c_str());
    }
}

int sc_main(int argc, char* argv[]) {
    sc_signal<bool> a, b, cin;
    sc_signal<bool> sum, cout;
    Adder adder{"adder"};

    adder.a(a);
    adder.b(b);
    adder.cin(cin);
    adder.sum(sum);
    adder.cout(cout);

    struct Vec {
        bool a, b, cin, sum, cout;
    };
    const Vec tests[] = {
        {0, 0, 0, 0, 0}, {0, 0, 1, 1, 0}, {0, 1, 0, 1, 0}, {0, 1, 1, 0, 1},
        {1, 0, 0, 1, 0}, {1, 0, 1, 0, 1}, {1, 1, 0, 0, 1}, {1, 1, 1, 1, 1},
    };

    for (const auto& t : tests) {
        a = t.a;
        b = t.b;
        cin = t.cin;
        
        // SC_ZERO_TIME doesn't work, so we have to use a small unit of time
        sc_start(1, SC_NS);

        expect("Adder", sum.read(), cout.read(), t.sum, t.cout, t.a, t.b,
               t.cin);
    }

    printf("All adder test cases passed\n");

    return 0;
}
