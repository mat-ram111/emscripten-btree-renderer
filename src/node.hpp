#pragma once

#include <sstream>
#include <vector>
#include <iostream>

struct node {
    const static uint32_t THREAD = 1 << 31;
    uint32_t l = 0, r = 0, p = 0;
    double cx = 0.0, cy = 0.0;
};