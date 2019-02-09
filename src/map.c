#include "map.h"

void m_init(_map m, int val)
{
    for (int i = 0; i < MAP_SIZE; i++)
        m[i] = val;
}

void m_copy(_map from, _map to)
{
    for (int i = 0; i < MAP_SIZE; i++)
        to[i] = from[i];
}

int m_pos(int x, int y)
{
    return y * MAP_H + x;
}