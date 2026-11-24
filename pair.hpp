#pragma once

#include <cstddef>

namespace rpp {

/*
    [[no_unique_address]] enables EBO by not requiring the minimum single-byte required by unique addressing. 
*/
template <typename T1, typename T2>
struct pair {
    [[no_unique_address]] T1 first;
    [[no_unique_address]] T2 second;
};

} // namespace rpp 