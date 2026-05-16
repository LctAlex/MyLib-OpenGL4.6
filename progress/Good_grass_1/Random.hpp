#pragma once
#include <math.h>
#include <time.h>

void initRandom()
{
    srand(time(NULL));
}

int randomInt(int min, int max)
{
    return min + (rand() % max);
}

float randomFloat(float min, float max)
{
    return (max - min) * rand()/(float)RAND_MAX + min;
}