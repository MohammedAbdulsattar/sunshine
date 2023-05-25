#include <iostream>
#include "rlImGui.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

void DrawLine(int startPosX, int startPosY, int endPosX, int endPosY, Color color); // draw a line
void DrawCircle(int centerX, int centerY, float radius, Color color); // draw a circle
void DrawRectangle(int posX, int posY, int width, int height, Color color); // draw a rectangle

bool IsKeyPressed(int key); // Check if a key has been pressed once
bool IsKeyDown(int key); // Check if a key is being pressed
bool IsKeyReleased(int key); // Check if a key has been released once
bool IsKeyUp(int key); // Check if a key is NOT being pressed

Texture2D LoadTexture(const char* fileName); // Load texture from file into GPU memory (VRAM)
Sound LoadSound(const char* fileName); // Load sound from file

void SetMusicVolume(Music music, float volume); // Set volume for music (1.0 is max level)
void SetSoundVolume(Sound sound, float volume); // Set volume for a sound (1.0 is max level)

void DrawCircleV(Vector2 center, float radius, Color color); // Draw a color-filled circle (Vector version)
void DrawTriangle(Vector2 v1, Vector2 v2, Vector2 v3, Color color); // Draw a color-filled triangle (vertex in counter-clockwise order!)
Vector2 GetGestureDragVector(void); // Get gesture drag vector

bool CheckCollisionCircles(Vector2 center1, float radius1, Vector2 center2, float radius2); // Check collision between two circles
bool CheckCollisionCircleRec(Vector2 center, float radius, Rectangle rec); // Check collision between a circle and rectangle

void WaitTime(double seconds); // Wait for some time (halt program execution)
float GetFrameTime(void); // Get time in seconds for last frame drawn (delta time)
Image ImageCopy(Image image); // Create an image duplicate (useful for transformations)

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sunshine"); // Create Window
    SetTargetFPS(60); // Set FPS for game

	// Testing Lab 1 Change. This is the main code and it renders "Hello World!" while game application is still running

    Vector2 ballPosition = { (float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2 }; // Create a circular ball positioned at half the screen width and height
    Vector2 stationaryCircle = { 500, 200 }; // Create a circular stationary ball positioned at a specific spot on the screen
    Vector2 mouseCircle = { (float)SCREEN_WIDTH / 2, 200 }; // Create a circular ball, controlled by mouse, which can be moved around by the player
    Rectangle starionaryRectangle = { 50, 300, 100, 100 }; // Create a Stationary Rectangle
    Vector2 firstTriangleLine = { 100,600 }, secondTriangleLine = { 200,600 }, thirdTriangleLine = { 100,500 }; // Create Triangle Points
    // Contra Splash Background Kindly Yoinked for Free from https://steemit.com/gaming/@jameslashomb/classic-game-reviews-contra-the-original-run-and-gun-shooter
    // Combined with a still of the scrolling game background in Paint 3D
    Texture2D titleScreen = LoadTexture("../game/assets/textures/TitleScreen.png"); // Load Texture from File
    // Obstacles Kindly Yoinked for Free from https://opengameart.org/content/2d-obstacle-collection
    Image original = LoadImage("../game/assets/textures/obstacles.png"); // Create an Image from File
    Texture2D imageTexture = LoadTextureFromImage(original); // Convert the loaded image to a texture

    UnloadImage(original); // Unload the original image (since it's already converted to a texture)

    rlImGuiSetup(true); // Setup GUI IO
    InitAudioDevice(); // Setup Init Audio Device

    bool musicPaused = false; // bool check to see if music is paused
    bool useGUI = false; // bool check to see if GUI is being used
    // kindly stolen from youtube. Probably piracy. https://www.youtube.com/watch?v=k8z07I1fzbQ&ab_channel=Lyn-Topic
    Music mainTheme = LoadMusicStream("../game/assets/audio/Throw_Away_Your_Mask.mp3"); // Load Music from File
    // kindly ripped from youtube. Royalty free. https://www.youtube.com/watch?v=Wi23bXE-h54&list=PLv4nWDim3p_orIE0b4kNcqx1P5FoyeG1-&index=10&ab_channel=SoundEffects%E3%80%90FREE%E3%80%91
    Sound villainLaugh = LoadSound("../game/assets/audio/VILLAIN_LAUGH.wav"); // Load Sound from File

	while (!WindowShouldClose()) // While the window is still running and escape key or window 'x' button isn't pressed
    {
        BeginDrawing(); // start drawing
        ClearBackground(RAYWHITE);

        DrawFPS(10, 10); // Display Game FPS

        DrawText("Hello World!", 500, 9, 20, RED); // basic output

    	DrawLine(50, 150, 150, 250, { 128,0,128,255 }); // draw a basic line
        DrawCircle(250, 250, 50, { 128,128,0,255 }); // draw a basic circle
        DrawRectangle(350, 350, 200, 200, { 0,128,128,255 }); // draw a basic rectangle
        DrawTriangle(firstTriangleLine, secondTriangleLine, thirdTriangleLine, PINK); // draw a basic triangle

        DrawText("move the Ball Circle with WASD keys", 500, 36, 20, DARKGRAY); // instructions for moving ball
        DrawText("Press V to play sound effect (Villain Laugh)", 500, 56, 20, DARKGRAY); // instructions for sound effect
        DrawText("Press P to pause the game for 5 seconds", 500, 76, 20, DARKGRAY); // instructions for pause
        DrawText("Press F1 to clear the background screen", 500, 700, 20, DARKGRAY); // instructions for clearing screen
        DrawCircleV(ballPosition, 50, BLUE); // Draw a vector circle with a center, set radius and color which can be moved with key input

        DrawText("Stationary Circle", 420, 120, 20, RED); // basic output for stationary circle
        DrawCircleV(stationaryCircle, 50, GREEN); // Draw a vector circle with a center, set radius and color which is stationary

        Color mouseCircleColor = RED; // Set the color for the Mouse Circle that will be used
        Color collisionColorStationary = YELLOW; // Yellow for Red and Green Collision
        Color collisionColorMoving = MAGENTA; // Magenta for Blue and Red Collision
        Color collisionColorWASD = Color{ 0,128,128,255 }; // Teal for Blue and Green Collision
        Color collisionrectangle = MAROON; // Maroon for Red and Brown Collision
        Color collisiontriangle = Color{89, 31, 60, 255}; // Raspberry Pink for Red and Pink Collision

        if (IsKeyDown(KEY_W))
        {
            ballPosition.y -= 2.0f; // move up, pixel precision
        }
        if (IsKeyDown(KEY_S))
        {
            ballPosition.y += 2.0f; // move down, pixel precision
        }
        if (IsKeyDown(KEY_A))
        {
            ballPosition.x -= 2.0f; // move left, pixel precision
        }
        if (IsKeyDown(KEY_D))
        {
            ballPosition.x += 2.0f; // move right, pixel precision
        }

		DrawTexture(titleScreen,700, 200, { 255,255,255,200 }); // Draw the Loaded Texture from file
        DrawTexture(imageTexture, 0, 0, WHITE); // Draw the loaded texture on the screen created from image file

        int mouse_x;
        int mouse_y;

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))  // Check for left mouse button clicks and display the current XY position for the mouse
        {

            mouse_x = GetMouseX();
            std::cout << "Mouse X: " << mouse_x << std::endl;
            mouse_y = GetMouseY();
			std::cout << "Mouse Y: " << mouse_y << std::endl;

        }

        mouseCircle = GetMousePosition(); // Poll the Mouse Movement, Getting XY position
        DrawCircleV(mouseCircle, 40, mouseCircleColor); // Draw a vector circle with a center, set radius and color which can be moved with the mouse
        DrawRectangle(starionaryRectangle.x, starionaryRectangle.y, starionaryRectangle.width, starionaryRectangle.height, BROWN); // Draw a rectangle with a set color

        if (CheckCollisionCircles(ballPosition, 50, stationaryCircle, 50)) // Check Collision Between WASD Circle and Stationary Circle
        {
            std::cout << "Collision Between WASD Circle and Stationary Circle Detected!" << std::endl;
            DrawCircleV(ballPosition, 50, collisionColorWASD); // Draw a vector circle with a center, set radius and color which is moving using WASD
            DrawCircleV(stationaryCircle, 50, collisionColorWASD); // Draw a vector circle with a center, set radius and color which is stationary
        }

        if (CheckCollisionCircles(mouseCircle, 40, stationaryCircle, 50)) // Check Collision Between Mouse Circle and Stationary Circle
        {
            std::cout << "Collision Between Stationary Circle and Mouse Circle Detected!" << std::endl;
            DrawCircleV(mouseCircle, 40, collisionColorStationary); // Draw a vector circle with a center, set radius and color which is moving using the mouse
            DrawCircleV(stationaryCircle, 50, collisionColorStationary); // Draw a vector circle with a center, set radius and color which is stationary
        }

        if (CheckCollisionCircles(mouseCircle, 40, ballPosition, 50)) // Check Collision Between Mouse Circle and WASD Circle
        {
            std::cout << "Collision Between WASD Circle and Mouse Circle Detected!" << std::endl;
            DrawCircleV(mouseCircle, 40, collisionColorMoving); // Draw a vector circle with a center, set radius and color which is moving using the mouse
            DrawCircleV(ballPosition, 50, collisionColorMoving); // Draw a vector circle with a center, set radius and color which is moving using WASD
        }

        if (CheckCollisionCircleRec(mouseCircle, 40, starionaryRectangle))
        {
            std::cout << "Collision Between Stationary Rectangle and Mouse Circle Detected!" << std::endl;
            DrawCircleV(mouseCircle, 40, collisionrectangle); // Draw a vector circle with a center, set radius and color which is moving using the mouse
            DrawRectangle(starionaryRectangle.x, starionaryRectangle.y, starionaryRectangle.width, starionaryRectangle.height, collisionrectangle); // Draw a vector rectangle with an x, y, width, height and color which is stationary
        }

        if (CheckCollisionPointTriangle(mouseCircle, firstTriangleLine, secondTriangleLine, thirdTriangleLine))
        {
            std::cout << "Collision Between Stationary Triangle and Mouse Circle Detected!" << std::endl;
            DrawCircleV(mouseCircle, 40, collisiontriangle); // Draw a vector circle with a center, set radius and color which is moving using the mouse
            DrawTriangle(firstTriangleLine, secondTriangleLine, thirdTriangleLine, collisiontriangle); // Draw a vector Triangle with 3 (x,y) point lines and color which is stationary
        }

        SetMusicVolume(mainTheme, 0.1f); // Set volume for music (1.0 is max level)
        SetSoundVolume(villainLaugh, 0.1f); // Set volume for a sound (1.0 is max level)

        UpdateMusicStream(mainTheme); // Update music and continue playing through the music track

        if (musicPaused)
        {
            PauseMusicStream(mainTheme); // Pause music when paused
        }
        else
        {
            PlayMusicStream(mainTheme); // Continue playing music
        }

        if (IsKeyPressed(KEY_V))
        {
            PlaySound(villainLaugh); // Play the sound effect when the Key is pressed
        }

        if (IsKeyPressed(KEY_F1))
        {
            ClearBackground(GRAY); // Clear the background image manually through a button press with a specified color, used for creating a new canvas or rendering purposes
        }

        DrawRectangle(970, 10, 300, 30, { 200, 150, 200, 255 });
        DrawText("Press ~ to open/close GUI", 980, 15, 20, WHITE); // GUI Instructions

        if (IsKeyPressed(KEY_GRAVE))
        {
            useGUI = !useGUI; // Flip trigger for GUI Render when Tilde (~) Key is pressed
        }

        if (useGUI)
        {
            rlImGuiBegin(); // Render GUI

            if (ImGui::Button("Play Villain Laugh"))
                PlaySound(villainLaugh); // Play the sound Effect

            if (musicPaused)
            {
                if (ImGui::Button("Play Main Menu Music"))
                    musicPaused = false; // Set music to unpause and play
            }
            else
            {
                if (ImGui::Button("Pause Main Menu Music"))
                    musicPaused = true; // Set music to pause and stop playing
            }

            if (ImGui::Button("Restart Main Menu Music"))
                SeekMusicStream(mainTheme, 0.0f); // Restart the music track to the beginning

            rlImGuiEnd(); // End GUI Render
        }

        if (IsKeyPressed(KEY_P))
        {
            std::cout << "Frame Time: " << GetFrameTime() << std::endl; // Display Current Frame Time for The Last Frame Drawn (Milliseconds of a single Second)
            WaitTime(5.00); // Pause Game for a 5 Second Timer
        }

        EndDrawing(); // end drawing
    }

    UnloadTexture(imageTexture); // Unload the image texture
    UnloadTexture(titleScreen); // Unload the image texture
	UnloadSound(villainLaugh); // Unload Sound Effect
    UnloadMusicStream(mainTheme); // Unload Music Track
    CloseAudioDevice(); // Close Audio
    rlImGuiShutdown(); // shut down GUI
    CloseWindow(); // close the window and unload OpenGL
    return 0;
}