# Roadmap of changes
I want to refactor this project to make it more useful, to the point where even I'd use it.

I think capturing specialized and unique data structures and reusing them is pretty nice... having this as a repository of them makes it easy for you to pick the proper data structure (or let AI decide for you) and from there it can extend that or rewrite it.

# Goals
## Preserve today's implementations, reimplement piece by piece
I want to keep what I have, but slowly walk through reimplementations. These will be in C++20.

Then I can force myself to relearn as well :)

## Be a reusable C++ library
Downstream projects should be able to consume replusplus like a CMake package.

```c++
#include <replusplus/vector.hpp>
#include <replusplus/map.hpp>
#include <replusplus/optional.hpp>
```

with CMake support integrating like

```
find_package(replusplus CONFIG REQUIRED)
target_link_libraries(my_target PRIVATE replusplus::replusplus)
```

## Better completeness of comparisons and testing/validation
On the comparison front, I feel like each component which mimics a STL container has a very clear ground truth for testing and validation.

In cases where there are not, I want to work to have better testing.

## Perf comparison
I want to learn and use Google Benchmark to see where I stand up against standard-library equivalents where possible.

This may extend to my unique primitives, not sure. I want to learn how to make fair comparisons here.

## Better support for my use cases in graphics/systems
Today I have a few structures (e.g., `gpu_array`) which support me specifically.

I want to make more of those. Especially for graphics projects I have in plan.

# Action items
## 1) Overhaul file structure and build system/CMake to support macOS and Linux building. Today we error on macOS because of CUDA for example, and that is bad.

I want this structure

```
include/
    vector.hpp
    optional.hpp
    ...

tests/
    vector_test.cpp
    optional_test.cpp
    ...

benchmark/
    vector_bench.cpp
    optional_bench.cpp
    ...

examples/
    vector_example.cpp
    optional_example.cpp
    ...

exploratory/
    gpu_array/
    systemc/
```

and we should also be cognizant of proper debug/release build stuff.

TODO