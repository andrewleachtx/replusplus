# replusplus
Reimplementing C++ STL containers and whatever else is fun.

## Structure
```
replusplus/
├── README.md
├── CMakeLists.txt
├── vendor/
│   └── googletest/
├── vector/
│   ├── CMakeLists.txt
│   ├── vector.hpp
│   └── vector.cpp
└── optional/
    ├── CMakeLists.txt
    ├── optional.hpp
    └── optional.cpp
```

There is a folder per target or structure with its own implementation (in-header, so `xyz.hpp`) and tests (`xyz.cpp`).

Basically, the top level `CMakeLists.txt` can be used to pick targets for tests, and then each target has its own `CMakeLists.txt` as a subdirectory.

TODO(A): Probably need a way to export libs later
