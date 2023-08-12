#pragma once

#include "Helper.h"
#include <vector>

template <typename T>
static void spliceVectorAtIndex(std::vector<T>& vector, int index)
{
    vector.erase(vector.begin() + index);
}

struct compare
{
    int key;
    compare(int const& i) : key(i) {}

    bool operator()(int const& i) {
        return (i == key);
    }
};

template <typename T>
static int getIndexOfElementInVector(std::vector<T>& vector, T& element)
{
    auto it = std::find(vector.begin(), vector.end(), element);

    if (it != vector.end()) {
        return static_cast<int>(std::distance(vector.begin(), it));
    }

    return -1;
}