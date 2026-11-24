# replusplus
Reimplementing C++ STL containers for learning sake.

## build/usage

Header only, C++20.

Configure and build:

```sh
cmake -S . -B build
cmake --build build
```

Run tests:

```sh
ctest --test-dir build --output-on-failure
```

or optionally through build. You can also pass a filter to typeo f test.

```sh
./build/replusplus_tests --gtest_filter='AllocatorTest.*'
```

Public headers are included through the `replusplus/` namespace (see `tests/` for examples)

```c++
#include <replusplus/vector.hpp>
#include <replusplus/optional.hpp>
```
