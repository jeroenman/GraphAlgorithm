#pragma once

#include "Helper.h"
#include <vector>

template <typename T>
static void spliceVectorAtIndex(std::vector<T>& vector, int index)
{
    vector.erase(vector.begin() + index);
}