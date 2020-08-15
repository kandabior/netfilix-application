//
// Created by orithaz on 11/24/19.
//

#include "../include/Tag.h"
#include <utility>

Tag::Tag(std::string name, int count) : tagName(std::move(name)), counter(count){}


bool Tag::operator<(Tag &other) {
    if (this->getCount() == other.getCount()) {
        return getName() < other.getName();
    } else {
        return getCount() < other.getCount();
    }
}

