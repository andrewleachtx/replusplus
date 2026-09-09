/*
    variant<int, double, std::string> my_variant;

    Forbids duplicates for now
*/
#include <algorithm>
#include <cstddef>

namespace fun {

template <typename... Types> class variant {
public:
    variant() {}

    // template <size_t I, Type> constexpr get() {
    //     // goal
    // }

private:
    // {sizeof(Types)...} expands to {sizeof(int), sizeof(double), sizeof(std::string)}
    static constexpr size_t max_size = std::max({sizeof(Types)...});
    static constexpr size_t max_align = std::max({alignof(Types)...});

    // Store a buffer of the largest Type provided, and interpret bytes as that
    alignas(max_align) char buffer_[max_size];

    // Store an index "tag" for what is currently active
};

// To find the index of each type we need a helper.
template <typename TargetType, typename... Types> constexpr size_t index_of() {
    static_assert(sizeof...(Types) > 0, "index_of<T, Types...> is empty");

    size_t idx = 0;
    bool is_found = false;
    size_t result_idx = 0;

    /*
        This syntax is a bit rough. But essentially, of form

        ( (expr), ...) -> ( (expr1), (expr2), (expr3) ) for all Types...
        
        where any "expr" is checking if the type T (target) is equal to the current Type

        std::is_same_v gets checked against all types
    */
    ((std::is_same_v<TargetType, Types>
          ? (is_found ? void() : (is_found = true, result_idx = idx), void())
          : void(),
      idx++),
     ...);

    // if (!is_found) {
    //     static_assert(std::dependent_false_v<T>,
    //                   "index_of<T, Types...>: T not found in Types...");
    // }

    return result_idx;
}

}; // namespace fun
