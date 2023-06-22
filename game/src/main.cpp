#include <iostream>
#include "rlImGui.h"
#include "raymath.h"
#include "raylib.h"
//#include "Math.h"
#include <vector>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

// Lab 4 - Part 1
enum class Tile
{
    Floor = 0, // Floor tiles can be walked on
    Wall, // Wall tiles cannot be walked on
    Count // number of Tile types (i.e. 2)
};

// Lab 4 - Part 1
#define MAP_WIDTH 16 // rows (down from origin or start)
#define MAP_HEIGHT 8 // columns (across from origin or start)

// Lab 4 - Part 8A
const Vector2 NORTH = { -1, 0 };
const Vector2 SOUTH = { 1, 0 };
const Vector2 EAST = { 0, 1 };
const Vector2 WEST = { 0, -1 };

// Lab 4 - Part 1 to Part 4
struct TileCoordinates
{
    int x = 0;
    int y = 0;

    TileCoordinates()
    {

    }

    TileCoordinates(int ax, int ay)
    {
        x = ax;
        y = ay;
    }

    TileCoordinates(float ax, float ay)
    {
        x = (int)ax;
        y = (int)ay;
    }

    TileCoordinates(Vector2 position)
    {
        x = (int)position.x;
        y = (int)position.y;
    }

    Vector2 toVec2() const
    {
        return Vector2{ (float)x, (float)y };
    }
};

// Lab 4 - Part 3
bool operator!=(const TileCoordinates& l, const TileCoordinates& r)
{
    return l.x != r.x && l.y != r.y; // use || or && for comparison, not sure?
}

bool operator==(const TileCoordinates& l, const TileCoordinates& r)
{
    return l.x == r.x && l.y == r.y; // use || or && for comparison, not sure?
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

// Lab 4 - Part 1 to Part 4
class Tilemap
{
public:
    int tileSizeX = 100; // Width of tiles in pixels when drawing
    int tileSizeY = 100; // Height of tiles in pixels when drawing
    Texture2D tileTextures[(int)Tile::Count]; // allocate an array with one Texture for each Tile type

    Tilemap()
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            for (int y = 0; y < MAP_HEIGHT; y++)
            {
                tiles[x][y] = Tile::Floor; // Or Tile::Wall, just initialize to something
            }
        }
        width = MAP_WIDTH; // Initialize width
        height = MAP_HEIGHT; // Initialize height
    }

    Vector2 GetScreenPosOfTile(TileCoordinates tilePosition) // convert from a tile to a screen position at top left corner of tile
    {
        return Vector2{ (float) tilePosition.x * tileSizeX, (float) tilePosition.y * tileSizeY };
    }

    TileCoordinates GetTileAtScreenPos(Vector2 positionOnScreen) // find a tile coordinate given a position on the screen over a tile
    {
        return { floorf(positionOnScreen.x / tileSizeX), floorf(positionOnScreen.y / tileSizeY) };
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

    void SetTile(int x, int y, Tile type) // set the tile at the specified coordinate in the grind
    {
        tiles[x][y] = type;
    }

    bool IsInsideGrid(int x, int y) // return true if the coordinate is inside the grid, false otherwise
    {
        return x >= 0 && x < width && y >= 0 && y < height;
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
    void DrawBorders(Color color = BLACK)
    {
        for (int x = 0; x < GetGridWidth(); x++)
            DrawLine(x * tileSizeX, 0, x * tileSizeX, GetGridHeight() * tileSizeY, color);

        for (int y = 0; y < GetGridHeight(); y++)
            DrawLine(0, y * tileSizeY,  GetGridWidth() * tileSizeX, y * tileSizeY, color);
    }

    void DrawTiles()
    {
	    for (int x = 0; x < GetGridWidth(); x++)
	    {
		    for (int y = 0; y < GetGridHeight(); y++)
		    {
                Tile tileType = tiles[x][y]; // get what type of tile is here
                Vector2 tilePosition = TilePosToScreenPos(x, y);
                Color colorToDraw = PINK;
                if (tileType == Tile::Floor)
                    colorToDraw = WHITE;
                else if (tileType == Tile::Wall)
                    colorToDraw = DARKGRAY;

                // Draw different colors at different positions based on the type of tile
                DrawRectangle(tilePosition.x, tilePosition.y, tileSizeX, tileSizeY, colorToDraw);
		    }
	    }
    }

	// Lab 4 - Part 5B
    void LoadTextures(const char* FloorTilePath, const char* WallTilePath)
    {
        tileTextures[(int)Tile::Floor] = LoadTexture(FloorTilePath);
        tileTextures[(int)Tile::Wall] = LoadTexture(WallTilePath);
    }

    void DrawTilesTextures()
    {
        for (int x = 0; x < GetGridWidth(); x++)
        {
            for (int y = 0; y < GetGridHeight(); y++)
            {
                Tile tileType = GetTile(x,y); // get what type of tile is here
                Vector2 tilePosition = TilePosToScreenPos(x, y);
                Texture tex = tileTextures[(int)tileType]; // use tile type as index in the tileTextures array
                Rectangle src = { 0,0, (float)tex.width, (float)tex.height };
                Rectangle dst = { tilePosition.x,tilePosition.y, (float)tileSizeX, (float)tileSizeY };
                DrawTexturePro(tex, src, dst, Vector2{ 0, 0 }, 0, WHITE);
            }
        }
    }

    // Lab 4 - Part 6
    int RegenerateLevel(float chanceOfWall = 0.2)
    {
        int wallCount = 0; // variable to keep track of the number of obstacles present

    	for (int x = 0; x < GetGridWidth(); x++)
	    {
		    for (int y= 0; y < GetGridHeight(); y++)
		    {
                if (GetRandomValue(0.0, 100) < chanceOfWall * 100)
                {
                    SetTile(x, y, Tile::Wall);
                    wallCount++; // increment the count of walls to keep track of how many obstacles are being placed
                }
                else
                    SetTile(x, y, Tile::Floor);
		    }
	    }

        return wallCount; // return the count of obstacles present
    }

    // Lab 4 - Part 7
    bool IsTraversible(Vector2 tilePosition)
    {
	    if (IsInsideGrid((int)tilePosition.x, (int)tilePosition.y))
	    {
            if (GetTile((int)tilePosition.x, (int)tilePosition.y) == Tile::Floor)
                return true;
	    }
        return false;
    }
    
    // Lab 4 - Part 8B
    std::vector<Vector2> GetTraversibleTilesAdjacentTo(Vector2 tilePos)
    {
        std::vector<Vector2> adjacentTilePositions;
        // North, South, East, West
        Vector2 N = { tilePos.x + NORTH.x, tilePos.y + NORTH.y };
        Vector2 S = { tilePos.x + SOUTH.x, tilePos.y + SOUTH.y };
        Vector2 E = { tilePos.x + EAST.x, tilePos.y + EAST.y };
        Vector2 W = { tilePos.x + WEST.x, tilePos.y + WEST.y };

        if (IsTraversible(N))
            adjacentTilePositions.push_back(N);
        if (IsTraversible(S))
            adjacentTilePositions.push_back(S);
        if (IsTraversible(W))
            adjacentTilePositions.push_back(W);
        if (IsTraversible(E))
            adjacentTilePositions.push_back(E);

        return adjacentTilePositions;
    }

private:
    Tile tiles[MAP_WIDTH][MAP_HEIGHT];

    // 2D vector of tiles (just IDs storing which type it is)
    // outer vector is to pick a column, inner vector is to pick a row from the column,
    // so to get to tile at xy position (0,6) across tiles [0][6]
    std::vector<std::vector<Tile>> tiles_vector; // Vector of Tiles - 2D dimension grid matrix of objects
    int width = 0; // number of columns
    int height = 0; // number of rows in each column
};


int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sunshine"); // Create Window
    SetTargetFPS(60); // Maximum Target FPS

    rlImGuiSetup(true); // Setup GUI Init
    bool useGUI = false; // bool check to see if GUI is being used

    Tilemap tilemap; // Create an instance of Tilemap
    // Texture Pack Files obtained from https://itch.io/bundle/download/6UMZoe6qhcj3_ccsNYMxVwLmiHxAe7Fe3yOG1ve7 (Purchased)
    // Original Texture Pack Link https://almostapixel.itch.io/small-burg-village-pack
    // Floor and Wall assets picked and edited out from sprite sheet for texture loading
    tilemap.LoadTextures("../game/assets/textures/Floor.png", "../game/assets/textures/Wall.png"); // Load textures

    int numberOfWalls = tilemap.RegenerateLevel(); // Initial population

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        float deltaTime = GetFrameTime(); // Get time in seconds for last frame drawn (delta time)

        tilemap.DrawTilesTextures(); // Draw the tilemap with textures
        tilemap.DrawBorders(); // Draw the tilemap borders

        if (IsKeyPressed(KEY_R)) // Replace 'KEY_R' with the key you want to use
        {
            numberOfWalls = tilemap.RegenerateLevel(); // Regenerate level on key press
        }

        DrawText("Tilemaps!", 16, 9, 20, RED);
        DrawRectangle(970, 10, 300, 30, { 200, 150, 200, 255 });
        DrawText("Press ~ to open/close GUI", 980, 15, 20, WHITE); // GUI Instructions

        if (IsKeyPressed(KEY_GRAVE))
        {
            useGUI = !useGUI; // Flip trigger for GUI Render when Tilde (~) Key is pressed
        }

        if (useGUI) // GUI window Open - Always place last before EndDrawing() so it always renders above everything else and not below
        {
            rlImGuiBegin(); // Start GUI

            ImGui::SliderInt("Adjust Tilemap Size X", &tilemap.tileSizeX, 1, 200, "%.0f"); // Slider for the tilemap sizeX
            ImGui::SliderInt("Adjust Tilemap Size Y", &tilemap.tileSizeY, 1, 200, "%.0f"); // Slider for the tilemap sizeY

            rlImGuiEnd(); // End GUI
        }

        std::cout << "--------------------------------------------------------" << std::endl; // Console outputs
        std::cout << "Frame DeltaTime: " << deltaTime << std::endl; // Console output to ensure delta time is functioning correctly according to frames
        std::cout << "TileMap Size: ( X: " << tilemap.tileSizeX << ", Y: " << tilemap.tileSizeY << ")" << std::endl; // Console output to check current tilemap sizes for X and Y
        std::cout << "Number of Obstacles ( Walls: " << numberOfWalls << ")" << std::endl; // Console output to check the number of wall obstacles
        std::cout << "--------------------------------------------------------" << std::endl; // Console outputs

        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();
    return 0;
}