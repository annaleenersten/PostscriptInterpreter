#include "interpreter.h"
#include "operations.h"
#include <iostream>
#include <functional>
#include <stdexcept>


/*
 * Controls whether the interpreter uses static or dynamic scoping.
 * - false: dynamic scoping (default behavior)
 * - true: static scoping (uses defining environment for code blocks)
 */
bool STATIC_SCOPING = false;

/*
 * Operand stack:
 * Holds all runtime values (integers, doubles, booleans, strings, etc.)
 * Used by all stack-based operations.
 */
std::vector<Value> op_stack;

/*
 * Dictionary stack:
 * Implements scoped environments.
 * Each PSDict represents a scope (like a symbol table).
 */
std::vector<PSDict*> dict_stack;

/*
 * Initializes the interpreter state.
 *
 * Responsibilities:
 *  - Clears any existing dictionary stack
 *  - Creates a global dictionary (top-level scope)
 *  - Registers all built-in operations as function entries
 *
 * All PostScript-like commands are mapped here.
 */
void init_interpreter() {
    dict_stack.clear();
    dict_stack.push_back(new PSDict(50));
    auto& dictionary = dict_stack.back()->dict;

    // Stack manipulation
    dictionary["exch"]   = std::function<void()>(exch_operation);
    dictionary["pop"]    = std::function<void()>(pop_operation);
    dictionary["copy"]   = std::function<void()>(copy_operation);
    dictionary["dup"]    = std::function<void()>(dup_operation);
    dictionary["clear"]  = std::function<void()>(clear_operation);
    dictionary["count"]  = std::function<void()>(count_operation);

    // Arithmetic
    dictionary["add"]    = std::function<void()>(add_operation);
    dictionary["sub"]    = std::function<void()>(sub_operation);
    dictionary["mul"]    = std::function<void()>(mul_operation);
    dictionary["div"]    = std::function<void()>(div_operation);
    dictionary["idiv"]   = std::function<void()>(idiv_operation);
    dictionary["mod"]    = std::function<void()>(mod_operation);

    dictionary["abs"]    = std::function<void()>(abs_operation);
    dictionary["neg"]    = std::function<void()>(neg_operation);
    dictionary["ceiling"]= std::function<void()>(ceiling_operation);
    dictionary["floor"]  = std::function<void()>(floor_operation);
    dictionary["round"]  = std::function<void()>(round_operation);
    dictionary["sqrt"]   = std::function<void()>(sqrt_operation);

    // Dictionary
    dictionary["dict"]      = std::function<void()>(dict_operation);
    dictionary["length"]    = std::function<void()>(length_operation);
    dictionary["maxlength"] = std::function<void()>(maxlength_operation);
    dictionary["begin"]     = std::function<void()>(begin_operation);
    dictionary["end"]       = std::function<void()>(end_operation);
    dictionary["def"]       = std::function<void()>(def_operation);

    // Strings
    dictionary["strlen"]  = std::function<void()>(string_length_operation); //length
    dictionary["get"]           = std::function<void()>(get_string_operation);
    dictionary["getinterval"]   = std::function<void()>(getinterval_operation);
    dictionary["putinterval"]   = std::function<void()>(putinterval_operation);

    // Boolean and Bitwise
    dictionary["eq"]     = std::function<void()>(eq_operation);
    dictionary["ne"]     = std::function<void()>(ne_operation);
    dictionary["ge"]     = std::function<void()>(ge_operation);
    dictionary["gt"]     = std::function<void()>(gt_operation);
    dictionary["le"]     = std::function<void()>(le_operation);
    dictionary["lt"]     = std::function<void()>(lt_operation);
    dictionary["and"]    = std::function<void()>(and_operation);
    dictionary["or"]     = std::function<void()>(or_operation);
    dictionary["not"]    = std::function<void()>(not_operation);
    dictionary["true"]   = std::function<void()>(true_operation);
    dictionary["false"]  = std::function<void()>(false_operation);

    // Flow control
    dictionary["if"]     = std::function<void()>(if_operation);
    dictionary["ifelse"] = std::function<void()>(ifelse_operation);
    dictionary["for"]    = std::function<void()>(for_operation);
    dictionary["repeat"] = std::function<void()>(repeat_operation);
    dictionary["quit"]   = std::function<void()>(quit_operation);

    // Input and Output
    dictionary["="]    = std::function<void()>(pop_print_operation); 
    dictionary["print"]      = std::function<void()>(print_operation);
    dictionary["=="]     = std::function<void()>(double_equals_operation);
}


/*
 * Looks up a token in the dictionary stack (scoped environment).
 *
 * Search order:
 *  - Top of stack → bottom (latest scope first)
 *
 * Behavior:
 *  - If token is a function → execute it immediately
 *  - If token is a code block → execute it (with optional static scoping)
 *  - If token is a literal → push onto operand stack
 *
 * Throws:
 *  - ParseFailed if token is not found in any scope
 *  - TypeMismatch for unsupported value types
 */

void lookup_in_dictionary(const std::string& input) {

    for (auto it = dict_stack.rbegin(); it != dict_stack.rend(); ++it) {

        PSDict* dict = *it;
        auto found = dict->dict.find(input);

        if (found == dict->dict.end()) continue;

        Value value = found->second;

        // -------------------------
        // functions
        // -------------------------
        if (std::holds_alternative<std::function<void()>>(value)) {
            std::get<std::function<void()>>(value)();
            return;
        }

        // -------------------------
        // code blocks
        // -------------------------
        if (std::holds_alternative<CodeBlock>(value)) {

            CodeBlock cb = std::get<CodeBlock>(value);

            if (STATIC_SCOPING) {
                dict_stack.push_back(cb.defining_env);
            }

            for (const auto& token : cb.code) {
                process_input(token);
            }

            if (STATIC_SCOPING) {
                dict_stack.pop_back();
            }

            return;
        }


        // -------------------------
        // literals
        // -------------------------
        if (std::holds_alternative<int>(value) ||
            std::holds_alternative<double>(value) ||
            std::holds_alternative<bool>(value) ||
            std::holds_alternative<std::string>(value)) {

            op_stack.push_back(value);
            return;
        }

        throw TypeMismatch("Unsupported value type in lookup");
    }

    throw ParseFailed("Could not find " + input);
}

/*
 * Main interpreter dispatch function.
 *
 * Execution order:
 *  1. Try parsing as a literal (number, string, boolean)
 *  2. If not a literal, try dictionary lookup
 *  3. If neither works, throw undefined token error
 *
 * This enforces a simple two-stage evaluation model.
 */
void process_input(const std::string& input) {

    // 1. FIRST: try constants (literals)
    try {
        Value v = process_constants(input);
        op_stack.push_back(v);
        return;
    }
    catch (ParseFailed&) {
        // not a literal
    }

    // 2. SECOND: try dictionary lookup (operations like def, add, begin)
    try {
        lookup_in_dictionary(input);
        return;
    }
    catch (const ParseFailed&) {
        // not found in dictionary
    }

    throw TypeMismatch("Undefined token: " + input);
}
