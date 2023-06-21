#include <iostream>
#include "rlImGui.h"
#include "raymath.h"
#include "raylib.h"
//#include "Math.h"
#include <vector>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

enum class Tile
{
    Floor = 0, // Floor tiles can be walked on
    Wall, // Wall tiles cannot be walked on
    Count // number of Tile types (i.e. 2)
};

#define MAP_WIDTH 16 // rows (down from origin or start)
#define MAP_HEIGHT 8 // columns (across from origin or start)

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

class Tilemap
{
public:
    int tileSizeX = 100; // Width of tiles in pixels when drawing
    int tileSizeY = 100; // Height of tiles in pixels when drawing

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
	    
    }

    int GetGridHeight() // get the number of rows in the grid
    {

    }

    Tile GetTile(int x, int y) // get the tile at the specified coordinate in the grid
    {
	    
    }

    void SetTile(int x, int y, Tile type) // set the tile at the specified coordinate in the grind
    {
	    
    }

    bool IsInsideGrid(int x, int y) // return true if the coordinate is inside the grid, false otherwise
    {
	    
    }

    Vector2 TilePosToScreenPos(Vector2 tilePosition) // convert from a tile coordinate to a screen position
    {
	    
    }

    Vector2 TilePosToScreenPos(int x, int y) // convert from a tile coordinate to a screen position
    {
	    
    }

    Vector2 ScreenPosToTilePos(Vector2 positionOnScreen) // find a tile coordinate given a position on the screen over a tile
    {
	    
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

bool operator!=(const TileCoordinates& l, const TileCoordinates& r)
{
    return l.x != r.x || l.y != r.y;
}

bool operator==(const TileCoordinates& l, const TileCoordinates& r)
{
    return l.x == r.x || l.y == r.y;
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


int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sunshine"); // Create Window
    SetTargetFPS(60); // Maximum Target FPS

    rlImGuiSetup(true); // Setup GUI Init
    bool useGUI = false; // bool check to see if GUI is being used

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Hello World!", 16, 9, 20, RED);

        float deltaTime = GetFrameTime(); // Get time in seconds for last frame drawn (delta time)

        DrawRectangle(970, 10, 300, 30, { 200, 150, 200, 255 });
        DrawText("Press ~ to open/close GUI", 980, 15, 20, WHITE); // GUI Instructions

        if (IsKeyPressed(KEY_GRAVE))
        {
            useGUI = !useGUI; // Flip trigger for GUI Render when Tilde (~) Key is pressed
        }

        if (useGUI) // GUI window Open
        {
            rlImGuiBegin(); // Start GUI

            ImGui::SliderFloat("Lab 4 Tilemap GUI", &deltaTime, 0, SCREEN_WIDTH, "%.3f", 0); // Slider for the 

            rlImGuiEnd(); // End GUI
        }



        EndDrawing();
    }

    CloseWindow();
    return 0;
}