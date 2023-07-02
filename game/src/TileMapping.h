#ifndef TILEMAPPING_H
#define TILEMAPPING_H

//#include "rlImGui.h"
//#include "raymath.h"
#include "raylib.h"
//#include "Math.h"
#include <vector>

#include "TileCoordinates.h"
#include "Character.h"

// Lab 4 - Part 1
#define MAP_WIDTH 16 // number of rows being set (down from origin or start)
#define MAP_HEIGHT 8 // number columns being set (across from origin or start)

// Lab 4 - Part 8A
const Vector2 NORTH = { -1, 0 }; // north direction vector
const Vector2 SOUTH = { 1, 0 }; // south direction vector
const Vector2 EAST = { 0, 1 }; // east direction vector
const Vector2 WEST = { 0, -1 }; // west direction vector

// Lab 5 - Part 8
const int HIGH_COST = 1000000; // High cost representing an obstacle or not traversable tile.

// Lab 4 - Part 1 to Part 4
class Tilemap
{
public:
    int tileSizeX = 80; // Width of tiles in pixels when drawing
    int tileSizeY = 90; // Height of tiles in pixels when drawing
    Texture2D tileTextures[(int)Tile::Count]; // allocate an array with one Texture for each Tile type

    Tilemap(); // constructor for initializing the tilemap by setting initial values for each tile according to width and height of grid

    Vector2 GetScreenPosOfTile(TileCoordinates tilePosition); // convert from a tile to a screen position at top left corner of tile

    TileCoordinates GetTileAtScreenPos(Vector2 positionOnScreen); // find a tile coordinate given a position on the screen over a tile

    int GetGridWidth(); // get the number of columns in the grid

    int GetGridHeight(); // get the number of rows in the grid

    Tile GetTile(int x, int y); // get the tile at the specified coordinate in the grid

    void SetTile(int x, int y, Tile type); // set the tile at the specified coordinate in the grid

    bool IsInsideGrid(int x, int y); // return true if the coordinate is inside the grid, false otherwise

    Vector2 TilePosToScreenPos(Vector2 tilePosition); // convert from a tile coordinate to a screen position

    Vector2 TilePosToScreenPos(int x, int y); // convert from a tile coordinate to a screen position

    Vector2 ScreenPosToTilePos(Vector2 positionOnScreen); // find a tile coordinate given a position on the screen over a tile

    // Lab 4 - Part 5A
    void DrawBorders(Color color); // Draw borders for the grid based on width and height

    void DrawTiles(); // Draw each tile

    // Lab 4 - Part 5B
    void LoadTextures(const char* FloorTilePath, const char* WallTilePath); // Load textures from file for floor tiles and wall tiles

    void DrawTilesTextures(); // Draw the texture tiles

    // Lab 4 - Part 6
    int RegenerateLevel(float chanceOfWall); // create walls based on a randomized number (chanceOfWall - from 0.00 no tiles are walls to 1.00 all tiles are walls)

    // Lab 4 - Part 7
    bool IsTraversable(Vector2 tilePosition); // check if the current tile XY position is inside the grid and traversable

    // Lab 4 - Part 8B
    std::vector<Vector2> GetTraversibleTilesAdjacentTo(TileCoordinates tilePos); // Dijkstra Algorithm & A* for a list of Nodes (vertexes) connected to a given node

    // Lab 4 - Homework Part 1
    void DrawAdjacency();

    // Lab 4 - Homework Part 2
    bool CanMove(Tilemap tilemap, Vector2 newPos, const Character& player); // accept a tilemap, vector position, and player character object

	// Lab 5 - Part 4
    std::vector<Vector2> GetAllTraversableTiles(); // Return a vector container of all XY traversable Tile positions available in current map

    // Lab 5 - Part 8
    int GetCostForTile(Vector2 tilePosition);
    bool ContainsTile(TileCoordinates tilePos); // Check to see if a tile is contained in the tilemap

private:
    Tile tiles[MAP_WIDTH][MAP_HEIGHT]; // create tiles based on the map width and height

    // 2D vector of tiles (just IDs storing which type it is)
    // outer vector is to pick a column, inner vector is to pick a row from the column,
    // so to get to tile at xy position (0,6) across tiles [0][6]
    std::vector<std::vector<Tile>> tiles_vector; // Vector of Tiles - 2D dimension grid matrix of objects
    int width = 0; // number of columns being (across the x coordinates)
    int height = 0; // number of rows being set (across the y coordinates)
};

#endif // TILEMAPPING_H