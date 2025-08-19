# replusplus
Reimplementing various components of the C++ STL. Maybe other stuff, e.g. linalg for gfx.

Testing with gtest, and using cppreference heavily. Headers are source.

# Building & Running Tests
Build
```sh
cmake -S . -B build
cmake --build build -j
```

Run
```sh

```

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