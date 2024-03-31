#include "util.h"

int random_integer(int lb, int ub)
{
    return (rand() % (ub - lb + 1)) + lb;
}