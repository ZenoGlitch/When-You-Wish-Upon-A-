#pragma once
#include <raylib.h>
#include <math.h>

enum TileType {Dirt = 0, Grass = 1, Rock = 2};

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}
