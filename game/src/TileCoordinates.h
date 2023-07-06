#ifndef TILE_COORDINATES_H
#define TILE_COORDINATES_H

//#include "rlImGui.h"
//#include "raymath.h"
#include "raylib.h"
#include <limits> // for std::numeric_limits
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

    // Lab 5 - Part 3
    void operator +=(const TileCoordinates& r); // new operator function for +=
    void operator -=(const TileCoordinates& r); // new operator function for -=
};

bool operator!=(const TileCoordinates& l, const TileCoordinates& r);
bool operator==(const TileCoordinates& l, const TileCoordinates& r);

TileCoordinates operator+(const TileCoordinates& l, const TileCoordinates& r);
TileCoordinates operator-(const TileCoordinates& l, const TileCoordinates& r);

TileCoordinates operator*(const TileCoordinates& l, const TileCoordinates& r);
TileCoordinates operator/(const TileCoordinates& l, const TileCoordinates& r);

// Lab 5 - Part 3
template <> struct std::hash<TileCoordinates> // Tell the compiler that you are providing a specialized "hash" Template for the "TileCoordinate" Data Type
{
    // Takes a "TileCoordinates" object and returns a "size_t" hash value.
    size_t operator() (const TileCoordinates& pos) const // Hash function which produces a unique number (hash) for each unique TileCoordinate
    {
        // calculate half-width based on the maximum value of an integer. Done to ensure a wide range of unique numbers (hash)
        const static long int halfWidth = sqrt(numeric_limits<int>::max());
        // calculate width based on the maximum value of an integer. Done to ensure a wide range of unique hashes
        const static long int width = halfWidth * 2;
        long long int x = pos.x; // store x position in a long long int variable for coordinates
        long long int y = pos.y; // store y position in a long long int variable for coordinates
        // Map the 2D coordinates to a 1D value by performing a calculation that ensures hash uniqueness
        // Similar to converting (row, column) into a single index in 1D array representation of a 2D array
        size_t hash = ((y + halfWidth) * width) + (x + halfWidth); // calculate the hash value based on the x & y coordinates.
        //size_t hash = ((x + y) * (x + y + 1) / 2) + y;
        return hash; // return the calculated hash value based on all the above
    }
};

#endif // TILE_COORDINATES_H