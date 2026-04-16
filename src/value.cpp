#include "value.h"

void print_value(const Value& v) {
    std::visit([](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;

        if constexpr (std::is_same_v<T, bool>) {
            std::cout << (arg ? "true" : "false");
        }
        else if constexpr (std::is_same_v<T, int>) {
            std::cout << arg;
        }
        else if constexpr (std::is_same_v<T, double>) {
            std::cout << arg;
        }
        else if constexpr (std::is_same_v<T, std::string>) {
            std::cout << arg;
        }
        else if constexpr (std::is_same_v<T, std::vector<std::string>>) {
            std::cout << "[ ";
            for (const auto& s : arg) std::cout << s << " ";
            std::cout << "]";
        }
        else if constexpr (std::is_same_v<T, std::function<void()>>) {
            std::cout << "<function>";
        }
        else if constexpr (std::is_same_v<T, PSDict*>) {
            std::cout << "<dict>";
        }
    }, v);
}