# replusplus
Reimplementing various components of the C++ STL. Maybe other stuff, e.g. linalg for gfx.

Testing with gtest, and using cppreference heavily. Headers are source.

# Building & Running Tests
The project was built on g++ with GNU 10.5 and CMake.

Run `build.sh` to create the available tests, and evaluate them with 

# Development
Format with `clang-format -i include/*.h tests/*.cpp`. The pre-commit hook checks for this.

# TODO
- [x] unique_ptr
- [x] shared_ptr
- [] weak_ptr
- [ ] stack
- [ ] vector
- [ ] list
- [ ] mutex
- [ ] condition_variable
- [ ] atomic
- [ ] semaphore
- [ ] map (rbtree)
- [ ] set
- [ ] thread
- [ ] linalg (matrix, vector, etc)
- [ ] hive
- [ ] deque
- [ ] string_view
- [ ] optional
- [ ] an allocator