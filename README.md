# replusplus
Reimplementing C++ STL containers and whatever else is fun.

## Structure
root
|_ README.md
|_ CMakeLists.txt
|_ vector/
    |_CMakeLists.txt
    |_vector.hpp
    |_vector.cpp
|_ optional/
    |_CMakeLists.txt
    |_optional.hpp
    |_optional.cpp

There is a folder per target or structure with its own implementation (in-header, so `xyz.hpp`) and tests (`xyz.cpp`).

Basically, the top level `CMakeLists.txt` can be used to pick targets for tests, and then each target has its own `CMakeLists.txt` as a subdirectory.
