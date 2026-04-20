#include "value.h"


/*
 * Prints a Value stored in the interpreter's variant type.
 *
 * This function uses std::visit to handle all possible types
 * stored inside the Value variant safely at runtime.
 *
 * Supported types:
 *  - bool
 *  - int
 *  - double
 *  - std::string
 *  - std::vector<std::string> (code representation)
 *  - CodeBlock (PostScript-like code block)
 *  - std::function<void()> (built-in operations)
 *  - PSDict* (dictionary objects / scopes)
 *
 * Each type is formatted appropriately for debugging output.
 */
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
            std::cout << "{ ";
            for (const auto& s : arg) std::cout << s << " ";
            std::cout << "}";
        }

        else if constexpr (std::is_same_v<T, CodeBlock>) {
            std::cout << "{ ";
            for (const auto& s : arg.code) std::cout << s << " ";
            std::cout << "}";
        }

        else if constexpr (std::is_same_v<T, std::function<void()>>) {
            std::cout << "<function>";
        }

        else if constexpr (std::is_same_v<T, PSDict*>) {
            std::cout << "<dict>";
        }
    }, v);
}