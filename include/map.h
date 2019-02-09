#ifndef _MAP_H_
#define _MAP_H_

#define MAP_W 32
#define MAP_H 32
#define MAP_SIZE MAP_W *MAP_H

typedef int _map[MAP_SIZE];

void m_init(_map m, int val);
void m_copy(_map from, _map to);
int m_pos(int x, int y);

#endif