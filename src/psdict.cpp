#include "psdict.h"
#include "parser.h"
#include <iostream>


/*
 * Constructor for PSDict (PostScript Dictionary / scope table).
 *
 * cap:
 *  Maximum capacity hint for this dictionary (not strictly enforced here,
 *  but may be used for optimization or future resizing logic).
 *
 * Each PSDict represents a single scope in the dictionary stack.
 */
PSDict::PSDict(int cap) {
    capacity = cap;
}

/*
 * Sets the parent dictionary (lexical scope link).
 *
 * This enables hierarchical lookup:
 *  - If a key is not found in the current dictionary,
 *    the search continues in the parent chain.
 */

void PSDict::set_parent(PSDict* p) {
    parent = p;
}

/*
 * Checks whether a key exists in the current dictionary chain.
 *
 * Search behavior:
 *  - Starts at the current dictionary
 *  - Traverses upward through parent scopes
 *  - Returns true if the key is found anywhere
 *  - Returns false if the key does not exist in any scope
 */
bool PSDict::contains(const std::string& key) const {
    const PSDict* current = this;

    while (current != nullptr) {
        if (current->dict.find(key) != current->dict.end()) {
            return true;
        }
        current = current->parent;
    }

    return false;
}

/*
 * Retrieves the value associated with a key from the dictionary chain.
 *
 * Lookup rules:
 *  - Searches current dictionary first
 *  - If not found, searches parent recursively
 *  - Continues until root dictionary is reached
 *
 * Returns:
 *  - The Value associated with the first matching key
 *
 * Throws:
 *  - std::out_of_range if the key does not exist in any scope
 */
Value PSDict::get(const std::string& key) const {
    const PSDict* current = this;

    while (current != nullptr) {
        auto it = current->dict.find(key);
        if (it != current->dict.end()) {
            return it->second;
        }
        current = current->parent;
    }

    throw std::out_of_range("not found");
}



