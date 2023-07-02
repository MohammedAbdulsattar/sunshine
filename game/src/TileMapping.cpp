#include "TileMapping.h"

Tilemap::Tilemap()
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

Vector2 Tilemap::GetScreenPosOfTile(TileCoordinates tilePosition)
{
    return Vector2{ (float)tilePosition.x * tileSizeX, (float)tilePosition.y * tileSizeY };
}

TileCoordinates Tilemap::GetTileAtScreenPos(Vector2 positionOnScreen)
{
    int tileX = static_cast<int>(positionOnScreen.x / tileSizeX);
    int tileY = static_cast<int>(positionOnScreen.y / tileSizeY);
    return TileCoordinates{ tileX, tileY };
    //return { floorf(positionOnScreen.x / tileSizeX), floorf(positionOnScreen.y / tileSizeY) }; // another way of checking for position on the screen over a tile
}

int Tilemap::GetGridWidth()
{
    return width;
}

int Tilemap::GetGridHeight()
{
    return height;
}

Tile Tilemap::GetTile(int x, int y)
{
    return tiles[x][y];
}

void Tilemap::SetTile(int x, int y, Tile type)
{
    tiles[x][y] = type;
}

bool Tilemap::IsInsideGrid(int x, int y)
{
    return x >= 0 && x < width&& y >= 0 && y < height;
}

Vector2 Tilemap::TilePosToScreenPos(Vector2 tilePosition)
{
    return Vector2{ tilePosition.x * tileSizeX, tilePosition.y * tileSizeY };
}

Vector2 Tilemap::TilePosToScreenPos(int x, int y)
{
    //return Vector2{ x * tileSizeX, y * tileSizeY }; // not correct way of doing this due to int-float issues requiring a static cast
    return Vector2{ static_cast<float>(x * tileSizeX), static_cast<float>(y * tileSizeY) };
}

Vector2 Tilemap::ScreenPosToTilePos(Vector2 positionOnScreen)
{
    return Vector2{ floorf(positionOnScreen.x / tileSizeX), floorf(positionOnScreen.y / tileSizeY) };
}

void Tilemap::DrawBorders(Color color)
{
    for (int x = 0; x < GetGridWidth(); x++)
        DrawLine(x * tileSizeX, 0, x * tileSizeX, GetGridHeight() * tileSizeY, color);

    for (int y = 0; y < GetGridHeight(); y++)
        DrawLine(0, y * tileSizeY, GetGridWidth() * tileSizeX, y * tileSizeY, color);
}

void Tilemap::DrawTiles()
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

void Tilemap::LoadTextures(const char* FloorTilePath, const char* WallTilePath)
{
    tileTextures[(int)Tile::Floor] = LoadTexture(FloorTilePath); // call LoadTexture function from file into GPU memory (VRAM) for Floor
    tileTextures[(int)Tile::Wall] = LoadTexture(WallTilePath); // call LoadTexture function from file into GPU memory (VRAM) for Wall
}

void Tilemap::DrawTilesTextures()
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

int Tilemap::RegenerateLevel(float chanceOfWall)
{
    int wallCount = 0; // variable to keep track of the number of obstacles present

    for (int x = 0; x < GetGridWidth(); x++) // across width (x positions)
    {
        for (int y = 0; y < GetGridHeight(); y++) // across height (y positions)
        {
            if (GetRandomValue(0, 100) < chanceOfWall * 100)
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

bool Tilemap::IsTraversable(Vector2 tilePosition)
{
    if (IsInsideGrid((int)tilePosition.x, (int)tilePosition.y)) // if current XY position is inside the grid
    {
        if (GetTile((int)tilePosition.x, (int)tilePosition.y) == Tile::Floor) // if the current tile XY position inside the grid is a floor tile
            return true; // return as true
    }
    return false; // otherwise false
}

std::vector<Vector2> Tilemap::GetTraversibleTilesAdjacentTo(TileCoordinates tilePos) // (Changed from Vector2 to TileCoordinates - Possibly Bug Prone)
{
    std::vector<Vector2> adjacentTilePositions; // create a vector container to store adjacent tile positions
    // North, South, East, West
    Vector2 N = { static_cast<float>(tilePos.x) + NORTH.x, static_cast<float>(tilePos.y) + NORTH.y }; // calculate the position of the tile to the north
    Vector2 S = { static_cast<float>(tilePos.x) + SOUTH.x, static_cast<float>(tilePos.y) + SOUTH.y }; // calculate the position of the tile to the south
    Vector2 E = { static_cast<float>(tilePos.x) + EAST.x, static_cast<float>(tilePos.y) + EAST.y }; // calculate the position of the tile to the east
    Vector2 W = { static_cast<float>(tilePos.x) + WEST.x, static_cast<float>(tilePos.y) + WEST.y }; // calculate the position of the tile to the west

    // Check if each adjacent tile is traversible and add it to the vector container if it is using element push_back
    if (IsTraversable(N))
        adjacentTilePositions.push_back(Vector2{N.x,N.y}); // Add the north tile position to the vector if it is traversable
    if (IsTraversable(S))
        adjacentTilePositions.push_back(Vector2{ S.x,S.y }); // Add the south tile position to the vector if it is traversable
    if (IsTraversable(W))
        adjacentTilePositions.push_back(Vector2{ W.x,W.y }); // Add the west tile position to the vector if it is traversable
    if (IsTraversable(E))
        adjacentTilePositions.push_back(Vector2{ E.x,E.y }); // Add the east tile position to the vector if it is traversable

    return adjacentTilePositions; // return the vector container of adjacent traversable positions
}

void Tilemap::DrawAdjacency()
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

bool Tilemap::CanMove(Tilemap tilemap, Vector2 newPos, const Character& player)
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

// Lab 5 - Part 4
std::vector<Vector2> Tilemap::GetAllTraversableTiles() // Return a vector container of all XY traversable Tile positions available in current map
{
    std::vector<Vector2> traversableTiles; // create a vector container of XY vectors to store the positions of all traversable tiles

    for (int x = 0; x < GetGridWidth(); x++) // go across the width of grid (columns)
    {
        for (int y = 0; y < GetGridHeight(); y++) // go down the height of grid (rows)
        {
            Vector2 tilePosition = { static_cast<float>(x), static_cast<float>(y) }; // create a temporary vector to represent the XY position of the current tile
            if (IsTraversable(tilePosition)) // if the tile at this current tile XY position is traversable
            {
                traversableTiles.push_back(tilePosition); // add this position to the vector container list of traversable XY position tiles
            }
        }
    }
    return traversableTiles; // return the vector container list of traversable tiles
}

// Lab 5 - Part 8
int Tilemap::GetCostForTile(Vector2 tilePosition)
{
    if (IsInsideGrid((int)tilePosition.x, (int)tilePosition.y))
    {
        Tile tileType = GetTile((int)tilePosition.x, (int)tilePosition.y);
        if (tileType == Tile::Floor)
        {
            return 1; // cost for moving through floor tile by 1
        }
        if (tileType == Tile::Wall)
        {
            return HIGH_COST; // cost for moving through wall tile (High Number similar to INFINITY)
        }
    }
    return HIGH_COST; // If tile is not inside the grid, then it is not traversable.
}

bool Tilemap::ContainsTile(TileCoordinates tilePos)
{
    // Implement logic to check if tilePos is inside the bounds of the tilemap
    return (tilePos.x >= 0 && tilePos.x < width && tilePos.y >= 0 && tilePos.y < height);
}