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

// Assignment 2 - Part 6
#include "Monster.h"

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
    // Assignment 2 - Part 6
    // Minotaur Texture obtained from https://clipart-library.com/minotaur-cliparts.html & https://clipart-library.com/clipart/minotaur-cliparts_13.htm
    Monster minotaur("../game/assets/textures/Monster.png", Vector2{1200,630 }); // Load the monster texture from file and give it a starting XY position vector

    int numberOfWalls = tilemap.RegenerateLevel(0.15); // Initial population of floors and walls (needs to be done beforehand to ensure proper memory initialization and avoid illegal memory access)

    // Lab 5 - Part 7
    PathFinder player_pathfinder; // PathFinder class object - runs through tilemap and finds the goal endNode from the character startNode
    // Assignment 2 - Part 7
    PathFinder minotaur_pathfinder; // PathFinder class object - runs through tilemap and finds the goal endNode from the minotaur startNode
	TileCoordinates mouseTilePos; // TileCoordinate mouseTilePos object - allows picking of a tile on grid base on XY mouse position
    // Lab 5 - Homework Part 1
	bool drawPathInfo = false; // set the DrawInfo to false and switch between on or off with a button trigger flag

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        float deltaTime = GetFrameTime(); // Get time in seconds for last frame drawn (delta time)

        tilemap.DrawTilesTextures(); // Draw the tilemap with textures
        tilemap.DrawAdjacency(); // Draw the tilemap adjacencies for floor tiles
        tilemap.DrawBorders({ BLACK }); // Draw the tilemap borders
        player.Draw(); // Draw the player character
        minotaur.Draw(); // Draw the Monster minotaur

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

        TileCoordinates minotaurTilePos = tilemap.GetTileAtScreenPos(minotaur.position);

        if (tilemap.ContainsTile(mouseTilePos))
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) // Solve run-through
            {
                player_pathfinder = PathFinder(&tilemap, playerTilePos, TileCoordinates(mouseTilePos));
                player_pathfinder.SolvePath(); // comment to allow step solving by tile for the player_pathfinder
            }

            if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) // Debug run-through
            {
                minotaur_pathfinder = PathFinder(&tilemap, minotaurTilePos, TileCoordinates(mouseTilePos));
                minotaur_pathfinder.SolvePath(); // comment to allow step solving by tile for the minotaur_pathfinder
            }
        }

        // Lab 5 - Homework Part 1
        if (IsKeyPressed(KEY_T)) // Check if the 'T' key was pressed
        {
            drawPathInfo = !drawPathInfo; // Toggle the value of drawPathInfo
        }

        if (player_pathfinder.map != nullptr)
        {
	        if (IsKeyPressed(KEY_SPACE)) // Debug run-through iteration (per node step evaluation)
	        {
                std::cout << "player step tile pathfinder is solving!";
	        	player_pathfinder.ProcessNextIterationFunctional();
	        }

        	if (drawPathInfo) // check if drawPathInfo flag is triggered
            {
                player_pathfinder.DrawCurrentState({255,105,180,128});
            }
        }

        // Assignment 2 - Part 7
        if (minotaur_pathfinder.map != nullptr)
        {
            if (IsKeyPressed(KEY_E)) // Debug run-through iteration (per node step evaluation)
            {
                std::cout << "minotaur step tile pathfinder is solving!";
            	minotaur_pathfinder.ProcessNextIterationFunctional();
            }

            if (drawPathInfo) // check if drawPathInfo flag is triggered
            {
                minotaur_pathfinder.DrawCurrentState({ 255,140,0,128 });
            }
        }

        DrawText("Tilemaps!", 16, 9, 20, { 255,255,255,255});
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

            ImGui::Text("Instructions for the Tilemap Functionality:");
            ImGui::Text("1. Use the above sliders to adjust the tilemap size in the window.");
            ImGui::Text("2. Press 'T' to toggle pathfinding info.");
            ImGui::Text("3. Press 'R' to regenerate the level.");
            ImGui::Text("4. Click 'Left Mouse button' to immediately solve player pathfinder.");
            ImGui::Text("5. Click 'Right Mouse button' to immediately solve monster pathfinder.");
            ImGui::Text("-> During Step Tile Solving:");
            ImGui::Text("6. Press 'SpaceBar' after 'Left Mouse' to solve player goal per tile.");
            ImGui::Text("7. Press 'E' after 'Right Mouse' to solve monster goal per tile.");

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