#ifndef TILE_COORDINATES_H
#define TILE_COORDINATES_H

//#include "rlImGui.h"
//#include "raymath.h"
#include "raylib.h"
//#include "Math.h"
#include <vector>

// Lab 4 - Part 1
enum class Tile
{
    Floor = 0, // Floor tiles can be walked on
    Wall, // Wall tiles cannot be walked on
    Count // number of Tile types (i.e. 2)
};

struct TileCoordinates
{
    int x = 0;
    int y = 0;

    TileCoordinates();
    TileCoordinates(int ax, int ay);
    TileCoordinates(float ax, float ay);
    TileCoordinates(Vector2 position);

    Vector2 toVec2() const;
};

bool operator!=(const TileCoordinates& l, const TileCoordinates& r);
bool operator==(const TileCoordinates& l, const TileCoordinates& r);
TileCoordinates operator+(const TileCoordinates& l, const TileCoordinates& r);
TileCoordinates operator-(const TileCoordinates& l, const TileCoordinates& r);
TileCoordinates operator*(const TileCoordinates& l, const TileCoordinates& r);
TileCoordinates operator/(const TileCoordinates& l, const TileCoordinates& r);

#endif // TILE_COORDINATES_H