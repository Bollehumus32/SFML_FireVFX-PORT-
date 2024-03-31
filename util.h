#ifndef FIREVFX_UTIL_H
#define FIREVFX_UTIL_H
#include <algorithm>
#include <random>

template <typename T>
T random_choice(const std::vector<T>& elements)
{
    std::random_device rd; // Obtain a random number from hardware
    std::mt19937 gen(rd()); // Seed the generator
    std::uniform_int_distribution<> distr(0, elements.size() - 1); // Define the range
    return elements[distr(gen)]; // Return a random element
}

int random_integer(int lb, int ub);

#endif //FIREVFX_UTIL_H
