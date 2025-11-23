#pragma once

#include <numbers>

double rand_double();
double rand_double(double low, double high);  // (low, high]

constexpr double DEG_TO_RAD = std::numbers::pi / 180;
constexpr int BITS_TO_BYTES = 8;
