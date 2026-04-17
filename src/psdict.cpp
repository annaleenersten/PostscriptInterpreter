#include "psdict.h"
#include "parser.h"
#include <iostream>


PSDict::PSDict(int cap) {
    capacity = cap;
}

void PSDict::set_parent(PSDict* p) {
    parent = p;
}

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
