#include "TileCoordinates.h"

TileCoordinates::TileCoordinates() {}

TileCoordinates::TileCoordinates(int ax, int ay) : x(ax), y(ay) {}

TileCoordinates::TileCoordinates(float ax, float ay) : x((int)ax), y((int)ay) {}

TileCoordinates::TileCoordinates(Vector2 position) : x((int)position.x), y((int)position.y) {}

Vector2 TileCoordinates::toVec2() const
{
    return Vector2{ (float)x, (float)y };
}

// Lab 5 - Part 3
void TileCoordinates::operator +=(const TileCoordinates& r) // new operator function for +=
{
	x += r.x;
	y += r.y;
}
void TileCoordinates::operator -=(const TileCoordinates& r) // new operator function for -=
{
	x -= r.x;
	y -= r.y;
}

// Assignment 2 - Part 6
TileCoordinates TileCoordinates::ScreenToTileCoordinates(const Vector2 position, const Vector2 tileSize)
{
    // return a TileCoordinate data type as screen coordinates of the position over tileSize XY 
    // divide the position screen coordinate over the tileSize and convert the result to an integer to store as TileCoordinates
	return TileCoordinates(static_cast<int>(position.x / tileSize.x), static_cast<int>(position.y / tileSize.y));
}

Vector2 TileCoordinates::TileToScreenCoordinates(const TileCoordinates& tileCoord, const Vector2& tileSize)
{
    // Return a Vector2 data type as a screen space coordinate of the tileCoordinate by tileSize XY
    // multiply the tileCoordinate by tileSize and convert the result to a float to store as Vector2 
    return Vector2{ static_cast<float>(tileCoord.x) * tileSize.x, static_cast<float>(tileCoord.y) * tileSize.y };
}

bool operator!=(const TileCoordinates& l, const TileCoordinates& r)
{
    return l.x != r.x || l.y != r.y;
}

bool operator==(const TileCoordinates& l, const TileCoordinates& r)
{
    return l.x == r.x && l.y == r.y;
}

TileCoordinates operator+(const TileCoordinates& l, const TileCoordinates& r)
{
    return { l.x + r.x, l.y + r.y };
}

TileCoordinates operator-(const TileCoordinates& l, const TileCoordinates& r)
{
    return { l.x - r.x, l.y - r.y };
}

TileCoordinates operator*(const TileCoordinates& l, const TileCoordinates& r)
{
    return { l.x * r.x, l.y * r.y };
}

TileCoordinates operator/(const TileCoordinates& l, const TileCoordinates& r)
{
    return { l.x / r.x, l.y / r.y };
}