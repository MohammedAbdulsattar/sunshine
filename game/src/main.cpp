#include <iostream>
#include "rlImGui.h"
//#include "raymath.h"
#include "raylib.h"
//#include "Math.h"
#include <vector>

#include "Character.h" // Including character stuff
#include "TileCoordinates.h" // not necessary as it is already being used through TileMapping but written and commented anyways
#include "TileMapping.h" // Including TileMapping and TileCoordinates stuff
#include "PathFinder.h" // Including PathFinder stuff

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

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

    int numberOfWalls = tilemap.RegenerateLevel(0.15); // Initial population of floors and walls (needs to be done beforehand to ensure proper memory initialization and avoid illegal memory access)

    // Lab 5 - Part 7
    PathFinder pathfinder;
    TileCoordinates mouseTilePos;
	bool drawPathInfo = true; // or false, depending on default behavior you want

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        float deltaTime = GetFrameTime(); // Get time in seconds for last frame drawn (delta time)

        tilemap.DrawTilesTextures(); // Draw the tilemap with textures
        tilemap.DrawAdjacency(); // Draw the tilemap adjacencies for floor tiles
        tilemap.DrawBorders({ BLACK }); // Draw the tilemap borders
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
            Vector2 newPos = { player.position.x + moveAmount, player.position.y }; // create a new position vector based on the current player XY position and the direction they want to head towards (right)
            if (tilemap.CanMove(tilemap, newPos, player)) // pass in the tilemap, current position of the moving player, and the player object to CanMove check
            {
                player.position.x += moveAmount; // allow player to move in the right direction
            }
        }

        if (IsKeyPressed(KEY_R)) // Replace 'KEY_R' with the key you want to use
        {
            numberOfWalls = tilemap.RegenerateLevel(0.15); // Regenerate level on key press - creating new randomly placed floors and walls based on the randomizer of the walls in RegenerateLevel()
            tilemap.DrawBorders({ BLACK }); // Draw the tilemap borders
        }

    	// Lab 5 - Part 7
        mouseTilePos = tilemap.GetTileAtScreenPos(GetMousePosition());

        // Convert player's position from screen space (Vector2 float) to tile space (integer)
        TileCoordinates playerTilePos = tilemap.GetTileAtScreenPos(player.position);

        if (tilemap.ContainsTile(mouseTilePos))
        {
	        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) // Debug run-through
	        {
                pathfinder = PathFinder(&tilemap, playerTilePos, TileCoordinates(mouseTilePos));
	        }

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) // Solve run-through
            {
                pathfinder = PathFinder(&tilemap, playerTilePos, TileCoordinates(mouseTilePos));
                pathfinder.SolvePath();
            }
        }

        if (pathfinder.map != nullptr)
        {
	        if (IsKeyPressed(KEY_SPACE)) // Debug run-through iteration (per node step evaluation)
	        {
                pathfinder.ProcessNextIterationFunctional();
	        }

            if (drawPathInfo)
            {
                pathfinder.DrawCurrentState();
            }
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