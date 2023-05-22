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

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sunshine");
    SetTargetFPS(60);
    // Testing Lab 1 Change. This is the main code and it renders "Hello World!" while game application is still running
    Vector2 ballPosition = { (float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2 };

    while (!WindowShouldClose()) // While the window is still running and escape key or window 'x' button isn't pressed
    {
        BeginDrawing(); // start drawing
        ClearBackground(RAYWHITE);
        DrawText("Hello World!", 16, 9, 20, RED); // basic output
        DrawLine(50, 50, 150, 150, { 128,0,128,255 }); // draw a basic line
        DrawCircle(250, 250, 50, { 128,128,0,255 }); // draw a basic circle
        DrawRectangle(350, 350, 200, 200, { 0,128,128,255 }); // draw a basic rectangle

        DrawText("move the ball with arrow keys", 500, 36, 20, DARKGRAY); // instructions for moving ball 
        DrawCircleV(ballPosition, 50, PURPLE); // Draw a vector circle with a center, set radius and color which can be moved with key input
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
        
        EndDrawing(); // end drawing
    }

    CloseWindow(); // close the window and unload OpenGL
    return 0;
}