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

template <> struct std::hash<TileCoordinates>
{
	size_t operator() (const TileCoordinates& pos) const // Hash function which produces a unique number (hash) for each unique TileCoordinate
	{
        const static long int halfWidth = sqrt(numeric_limits<int>::max());
        const static long int width = halfWidth * 2;
        long long int x = pos.x;
        long long int y = pos.y;
        size_t hash = ((y + halfWidth) * width) + (x + halfWidth);
        //size_t hash = ((x + y) * (x + y + 1) / 2) + y;
        return hash;
	}
};

#endif // TILE_COORDINATES_H