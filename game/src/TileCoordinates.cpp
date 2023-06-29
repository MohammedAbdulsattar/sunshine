#include "TileCoordinates.h"

TileCoordinates::TileCoordinates() {}

TileCoordinates::TileCoordinates(int ax, int ay) : x(ax), y(ay) {}

TileCoordinates::TileCoordinates(float ax, float ay) : x((int)ax), y((int)ay) {}

TileCoordinates::TileCoordinates(Vector2 position) : x((int)position.x), y((int)position.y) {}

Vector2 TileCoordinates::toVec2() const
{
    return Vector2{ (float)x, (float)y };
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