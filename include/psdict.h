#pragma once

#include <unordered_map>
#include <string>
#include "value.h"

class PSDict {
public:
    std::unordered_map<std::string, Value> dict;
    PSDict* parent = nullptr;
    int capacity;

    PSDict(int cap);

    void set_parent(PSDict* p);
    bool contains(const std::string& key) const;
    Value get(const std::string& key) const;
};