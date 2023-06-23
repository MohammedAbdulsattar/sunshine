#include <iostream>
#include "rlImGui.h"
#include "raymath.h"
#include "raylib.h"
//#include "Math.h"
#include <vector>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

// Lab 4 - Homework Part 2
class Character
{
public:
    Texture2D sprite; // Texture to hold character sprite
    Vector2 position; // position vector for the character

    Character(const char* texturePath, Vector2 startPos)
    {
        sprite = LoadTexture(texturePath); // load texture using the provided texture path
        position = startPos; // set the position for character according to current position
    }

    void Draw()
    {
        DrawTextureEx(sprite, position, 0.0f, 3.0f, WHITE); // render out the character sprite at the current position
    }
};

// Lab 4 - Part 1
enum class Tile
{
    Floor = 0, // Floor tiles can be walked on
    Wall, // Wall tiles cannot be walked on
    Count // number of Tile types (i.e. 2)
};

// Lab 4 - Part 1
#define MAP_WIDTH 16 // number of rows being set (down from origin or start)
#define MAP_HEIGHT 8 // number columns being set (across from origin or start)

// Lab 4 - Part 8A
const Vector2 NORTH = { -1, 0 }; // north direction vector
const Vector2 SOUTH = { 1, 0 }; // south direction vector
const Vector2 EAST = { 0, 1 }; // east direction vector
const Vector2 WEST = { 0, -1 }; // west direction vector

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
    return l.x != r.x && l.y != r.y; // return a constant coordinate address for column != x and row != y (not equal)
}

bool operator==(const TileCoordinates& l, const TileCoordinates& r)
{
    return l.x == r.x && l.y == r.y; // return a constant coordinate address for column == x and row == y (equal)
}

TileCoordinates operator+(const TileCoordinates& l, const TileCoordinates& r)
{
    return { l.x + r.x, l.y + r.y }; // return a constant coordinate address for column + x and row + y (addition)
}

TileCoordinates operator-(const TileCoordinates& l, const TileCoordinates& r)
{
    return { l.x - r.x, l.y - r.y }; // return a constant coordinate address for column - x and row - y (subtraction)
}

TileCoordinates operator*(const TileCoordinates& l, const TileCoordinates& r)
{
    return { l.x * r.x, l.y * r.y }; // return a constant coordinate address for column * x and row * y (multiplication)
}

TileCoordinates operator/(const TileCoordinates& l, const TileCoordinates& r)
{
    return { l.x / r.x, l.y / r.y }; // return a constant coordinate address for column / x and row / y (division)
}

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
        return Vector2{ (float) tilePosition.x * tileSizeX, (float) tilePosition.y * tileSizeY };
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
    void DrawBorders(Color color = BLACK) // Draw borders for the grid based on width and height
    {
        for (int x = 0; x < GetGridWidth(); x++)
            DrawLine(x * tileSizeX, 0, x * tileSizeX, GetGridHeight() * tileSizeY, color);

        for (int y = 0; y < GetGridHeight(); y++)
            DrawLine(0, y * tileSizeY,  GetGridWidth() * tileSizeX, y * tileSizeY, color);
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
                Tile tileType = GetTile(x,y); // get what type of tile is here on the current grid position
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
		    for (int y= 0; y < GetGridHeight(); y++) // across height (y positions)
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
    tilemap.LoadTextures("../game/assets/textures/Floor.png", "../game/assets/textures/Wall.png"); // Load Floor and Wall textures from file

    // Player asset picked and edited out from sprite sheet for texture loading
    Character player("../game/assets/textures/Player.png", Vector2{ 0, 0 }); // Load the player texture from file and give it a starting XY position vector

    int numberOfWalls = tilemap.RegenerateLevel(); // Initial population of floors and walls (needs to be done beforehand to ensure proper memory initialization and avoid illegal memory access)

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        float deltaTime = GetFrameTime(); // Get time in seconds for last frame drawn (delta time)

        tilemap.DrawTilesTextures(); // Draw the tilemap with textures
        tilemap.DrawAdjacency(); // Draw the tilemap adjacencies for floor tiles
        tilemap.DrawBorders(); // Draw the tilemap borders
        player.Draw(); // Draw the player character

        float moveAmount = 1.0f;

        if (IsKeyDown(KEY_W) && player.position.y > 0)
        {
            Vector2 newPos = { player.position.x, player.position.y - moveAmount }; // create a new position vector based on the current player XY position and the direction they want to head towards (up)
            if (tilemap.CanMove(tilemap, newPos, player)) // pass in the tilemap, current position of the moving player, and the player object to CanMove check
            {
                player.position.y -= moveAmount; // allow player to move in the up direction
            }
        }

        if (IsKeyDown(KEY_S) && player.position.y < SCREEN_HEIGHT)
        {
            Vector2 newPos = { player.position.x, player.position.y + moveAmount }; // create a new position vector based on the current player XY position and the direction they want to head towards (down)
            if (tilemap.CanMove(tilemap, newPos, player)) // pass in the tilemap, current position of the moving player, and the player object to CanMove check
            {
                player.position.y += moveAmount; // allow player to move in the down direction
            }
        }

        if (IsKeyDown(KEY_A) && player.position.x > 0)
        {
            Vector2 newPos = { player.position.x - moveAmount, player.position.y }; // create a new position vector based on the current player XY position and the direction they want to head towards (left)
            if (tilemap.CanMove(tilemap, newPos, player)) // pass in the tilemap, current position of the moving player, and the player object to CanMove check
            {
                player.position.x -= moveAmount; // allow player to move in the left direction
            }
        }

        if (IsKeyDown(KEY_D) && player.position.x < SCREEN_WIDTH)
        {
            Vector2 newPos = { player.position.x + moveAmount, player.position.y  }; // create a new position vector based on the current player XY position and the direction they want to head towards (right)
            if (tilemap.CanMove(tilemap, newPos, player)) // pass in the tilemap, current position of the moving player, and the player object to CanMove check
            {
                player.position.x += moveAmount; // allow player to move in the right direction
            }
        }

        if (IsKeyPressed(KEY_R)) // Replace 'KEY_R' with the key you want to use
        {
            numberOfWalls = tilemap.RegenerateLevel(); // Regenerate level on key press - creating new randomly placed floors and walls based on the randomizer of the walls in RegenerateLevel()
            tilemap.DrawBorders(); // Draw the tilemap borders
        }

        DrawText("Tilemaps!", 16, 9, 20, RED);
        DrawRectangle(970, 10, 300, 30, { 200, 150, 200, 255 });
        DrawText("Press ~ to open/close GUI", 980, 15, 20, WHITE); // GUI Instructions

        if (IsKeyPressed(KEY_GRAVE)) // Detect GUI key trigger
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
        std::cout << "Player Position: ( X: " << player.position.x << ", Y: " << player.position.y << ")" << std::endl; // Console output to track current player XY position
        std::cout << "--------------------------------------------------------" << std::endl; // Console outputs

        EndDrawing(); // end all raylib drawings
    }

    rlImGuiShutdown(); // Shut down GUI
    CloseWindow(); // Close Window
    return 0;
}