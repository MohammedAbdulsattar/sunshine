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

// Lab 4 - Part 1 to Part 4
class Tilemap
{
public:
    int tileSizeX = 80; // Width of tiles in pixels when drawing
    int tileSizeY = 90; // Height of tiles in pixels when drawing
    Texture2D tileTextures[(int)Tile::Count]; // allocate an array with one Texture for each Tile type

    Tilemap() // constructor for initializing the tilemap by setting initial values for each tile according to width and height of grid
    {
        for (int x = 0; x < MAP_WIDTH; x++) // increment up to the preset width (columns)
        {
            for (int y = 0; y < MAP_HEIGHT; y++) // increment up to the preset height (rows)
            {
                tiles[x][y] = Tile::Floor; // Or Tile::Wall, just initialize to something
            }
        }
        width = MAP_WIDTH; // Initialize width
        height = MAP_HEIGHT; // Initialize height
    }

    Vector2 GetScreenPosOfTile(TileCoordinates tilePosition) // convert from a tile to a screen position at top left corner of tile
    {
        return Vector2{ (float)tilePosition.x * tileSizeX, (float)tilePosition.y * tileSizeY };
    }

    TileCoordinates GetTileAtScreenPos(Vector2 positionOnScreen) // find a tile coordinate given a position on the screen over a tile
    {
        int tileX = static_cast<int>(positionOnScreen.x / tileSizeX);
        int tileY = static_cast<int>(positionOnScreen.y / tileSizeY);
        return TileCoordinates{ tileX, tileY };
        //return { floorf(positionOnScreen.x / tileSizeX), floorf(positionOnScreen.y / tileSizeY) }; // another way of checking for position on the screen over a tile
    }

    int GetGridWidth() // get the number of columns in the grid
    {
        return width;
    }

    int GetGridHeight() // get the number of rows in the grid
    {
        return height;
    }

    Tile GetTile(int x, int y) // get the tile at the specified coordinate in the grid
    {
        return tiles[x][y];
    }

    void SetTile(int x, int y, Tile type) // set the tile at the specified coordinate in the grid
    {
        tiles[x][y] = type;
    }

    bool IsInsideGrid(int x, int y) // return true if the coordinate is inside the grid, false otherwise
    {
        return x >= 0 && x < width&& y >= 0 && y < height;
    }

    Vector2 TilePosToScreenPos(Vector2 tilePosition) // convert from a tile coordinate to a screen position
    {
        return Vector2{ tilePosition.x * tileSizeX, tilePosition.y * tileSizeY };
    }

    Vector2 TilePosToScreenPos(int x, int y) // convert from a tile coordinate to a screen position
    {
        //return Vector2{ x * tileSizeX, y * tileSizeY }; // not correct way of doing this due to int-float issues requiring a static cast
        return Vector2{ static_cast<float>(x * tileSizeX), static_cast<float>(y * tileSizeY) };
    }

    Vector2 ScreenPosToTilePos(Vector2 positionOnScreen) // find a tile coordinate given a position on the screen over a tile
    {
        return Vector2{ floorf(positionOnScreen.x / tileSizeX), floorf(positionOnScreen.y / tileSizeY) };
    }

    // Lab 4 - Part 5A
    void DrawBorders(Color color = BLACK) // Draw borders for the grid based on width and height
    {
        for (int x = 0; x < GetGridWidth(); x++)
            DrawLine(x * tileSizeX, 0, x * tileSizeX, GetGridHeight() * tileSizeY, color);

        for (int y = 0; y < GetGridHeight(); y++)
            DrawLine(0, y * tileSizeY, GetGridWidth() * tileSizeX, y * tileSizeY, color);
    }

    void DrawTiles() // Draw each tile
    {
        for (int x = 0; x < GetGridWidth(); x++) // across the X in columns
        {
            for (int y = 0; y < GetGridHeight(); y++) // across the Y in rows
            {
                Tile tileType = tiles[x][y]; // get what type of tile is here
                Vector2 tilePosition = TilePosToScreenPos(x, y); // create a new vector tile position based on current XY grid position
                Color colorToDraw = PINK; // set initial draw color
                if (tileType == Tile::Floor) // if tile type is a Floor
                    colorToDraw = WHITE; // set color to white
                else if (tileType == Tile::Wall) // if tile type is a Wall
                    colorToDraw = DARKGRAY; // set color to dark gray

                // Draw a different colored tile at each different XY position based on the type of tile on the grid
                DrawRectangle(tilePosition.x, tilePosition.y, tileSizeX, tileSizeY, colorToDraw);
            }
        }
    }

    // Lab 4 - Part 5B
    void LoadTextures(const char* FloorTilePath, const char* WallTilePath) // Load textures from file for floor tiles and wall tiles
    {
        tileTextures[(int)Tile::Floor] = LoadTexture(FloorTilePath); // call LoadTexture function from file into GPU memory (VRAM) for Floor
        tileTextures[(int)Tile::Wall] = LoadTexture(WallTilePath); // call LoadTexture function from file into GPU memory (VRAM) for Wall
    }

    void DrawTilesTextures() // Draw the texture tiles
    {
        for (int x = 0; x < GetGridWidth(); x++) // across columns in the X
        {
            for (int y = 0; y < GetGridHeight(); y++) // across rows in the Y
            {
                Tile tileType = GetTile(x, y); // get what type of tile is here on the current grid position
                Vector2 tilePosition = TilePosToScreenPos(x, y); // create a vector tile object at that current position
                Texture tex = tileTextures[(int)tileType]; // use tile type as index in the tileTextures array
                Rectangle src = { 0,0, (float)tex.width, (float)tex.height }; // double buffer method, draw a rectangle for the tile object at an XY position with a width and height as the source
                Rectangle dst = { tilePosition.x,tilePosition.y, (float)tileSizeX, (float)tileSizeY }; // double buffer method, render the rectangle destination for the tile object at an XY position with a width and height
                DrawTexturePro(tex, src, dst, Vector2{ 0, 0 }, 0, WHITE); // draw the tile using the type (texture), source, destination, position, rotation and color
            }
        }
    }

    // Lab 4 - Part 6
    int RegenerateLevel(float chanceOfWall = 0.15) // create walls based on a randomized number (chanceOfWall - from 0.00 no tiles are walls to 1.00 all tiles are walls)
    {
        int wallCount = 0; // variable to keep track of the number of obstacles present

        for (int x = 0; x < GetGridWidth(); x++) // across width (x positions)
        {
            for (int y = 0; y < GetGridHeight(); y++) // across height (y positions)
            {
                if (GetRandomValue(0.0, 100) < chanceOfWall * 100)
                {
                    SetTile(x, y, Tile::Wall); // set the current XY tile as a wall
                    wallCount++; // increment the count of walls to keep track of how many obstacles are being placed
                }
                else
                    SetTile(x, y, Tile::Floor); // otherwise set the current XY tile as a floor
            }
        }
        return wallCount; // return the count of obstacles present
    }

    // Lab 4 - Part 7
    bool IsTraversable(Vector2 tilePosition) // check if the current tile XY position is inside the grid and traversable
    {
        if (IsInsideGrid((int)tilePosition.x, (int)tilePosition.y)) // if current XY position is inside the grid
        {
            if (GetTile((int)tilePosition.x, (int)tilePosition.y) == Tile::Floor) // if the current tile XY position inside the grid is a floor tile
                return true; // return as true
        }
        return false; // otherwise false
    }

    // Lab 4 - Part 8B
    std::vector<Vector2> GetTraversibleTilesAdjacentTo(Vector2 tilePos) // Dijkstra Algorithm & A* for a list of Nodes (vertexes) connected to a given node
    {
        std::vector<Vector2> adjacentTilePositions; // create a vector container to store adjacent tile positions
        // North, South, East, West
        Vector2 N = { tilePos.x + NORTH.x, tilePos.y + NORTH.y }; // calculate the position of the tile to the north
        Vector2 S = { tilePos.x + SOUTH.x, tilePos.y + SOUTH.y }; // calculate the position of the tile to the south
        Vector2 E = { tilePos.x + EAST.x, tilePos.y + EAST.y }; // calculate the position of the tile to the east
        Vector2 W = { tilePos.x + WEST.x, tilePos.y + WEST.y }; // calculate the position of the tile to the west

        // Check if each adjacent tile is traversible and add it to the vector container if it is using element push_back
        if (IsTraversable(N))
            adjacentTilePositions.push_back(N); // Add the north tile position to the vector if it is traversable
        if (IsTraversable(S))
            adjacentTilePositions.push_back(S); // Add the south tile position to the vector if it is traversable
        if (IsTraversable(W))
            adjacentTilePositions.push_back(W); // Add the west tile position to the vector if it is traversable
        if (IsTraversable(E))
            adjacentTilePositions.push_back(E); // Add the east tile position to the vector if it is traversable

        return adjacentTilePositions; // return the vector container of adjacent traversable positions
    }

    // Lab 4 - Homework Part 1
    void DrawAdjacency()
    {
        for (int x = 0; x < GetGridWidth(); x++) // iterate through all the columns
        {
            for (int y = 0; y < GetGridHeight(); y++) // iterate through all the rows
            {
                if (GetTile(x, y) == Tile::Floor) // if current tile at position XY in the grid is a floor tile
                {
                    std::vector<Vector2> adjacents = GetTraversibleTilesAdjacentTo(Vector2{ (float)x, (float)y }); // create a container of vector objects based on tile positions
                    for (auto& tile : adjacents) // iterate through the array of vector objects
                    {
                        Vector2 currentCenter = Vector2{ (float)x * tileSizeX + tileSizeX / 2, (float)y * tileSizeY + tileSizeY / 2 }; // create a vector object at the current tile
                        Vector2 adjacentCenter = Vector2{ tile.x * tileSizeX + tileSizeX / 2, tile.y * tileSizeY + tileSizeY / 2 }; // create a vector object at the adjacent tile
                        DrawLineEx(currentCenter, adjacentCenter, 1.0f, GREEN); // draw a line from current tile to the adjacent tile
                        DrawCircle(currentCenter.x, currentCenter.y, 5.0f, GREEN); // draw a circle at the current tile
                    }
                }
            }
        }
    }

    // Lab 4 - Homework Part 2
    bool CanMove(Tilemap tilemap, Vector2 newPos, const Character& player) // accept a tilemap, vector position, and player character object
    {
        // create XY coordinates for the top left corner of the player
        TileCoordinates coordsTopLeft = tilemap.GetTileAtScreenPos(newPos);
        // create XY coordinates for the top right corner of the player based on the sprite width from the starting X position
        TileCoordinates coordsTopRight = tilemap.GetTileAtScreenPos({ newPos.x + player.sprite.width * 3.0f, newPos.y });
        // Create XY coordinates for the bottom left corner of the player based on the sprite height from the starting Y position
        TileCoordinates coordsBottomLeft = tilemap.GetTileAtScreenPos({ newPos.x, newPos.y + player.sprite.height * 3.0f });
        // Create XY coordinates for the bottom right corner of the player based on the sprite width and height from the starting XY position
        TileCoordinates coordsBottomRight = tilemap.GetTileAtScreenPos({ newPos.x + player.sprite.width * 3.0f, newPos.y + player.sprite.height * 3.0f });

        // return all coordinates of the player based on whether a tile the player currently is on can be traversed
        return tilemap.IsTraversable(Vector2{ (float)coordsTopLeft.x, (float)coordsTopLeft.y }) &&
            tilemap.IsTraversable(Vector2{ (float)coordsTopRight.x, (float)coordsTopRight.y }) &&
            tilemap.IsTraversable(Vector2{ (float)coordsBottomLeft.x, (float)coordsBottomLeft.y }) &&
            tilemap.IsTraversable(Vector2{ (float)coordsBottomRight.x, (float)coordsBottomRight.y });
    }

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