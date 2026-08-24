#include <iostream>
#include <type_traits>

template <typename F>
void test(F task)
{
    using ReturnType = std::invoke_result_t<F>;

    // Just for testing
    if constexpr (std::is_same_v<ReturnType, int>) {
        std::cout << "Return type is int\n";
    }
    else if constexpr (std::is_same_v<ReturnType, double>) {
        std::cout << "Return type is double\n";
    }
}

int main()
{
    test([] {
        return 42;
    });

    test([] {
        return 3.14;
    });

    return 0;
}