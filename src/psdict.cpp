#include "psdict.h"
#include <iostream>


void PSDict::set_parent(PSDict* p) {
    parent = p;
}

bool PSDict::contains(const std::string& key) const {
    return dict.find(key) != dict.end();
}

Value PSDict::get(const std::string& key) const {
    const PSDict* current = this;

    while (current != nullptr) {
        auto it = current->dict.find(key);
        if (it != current->dict.end()) {
            return it->second;
        }
        current = current->parent;
    }

    throw std::runtime_error("Undefined symbol: " + key);
}
