#include <iostream>
#include "rlImGui.h"
#include "raymath.h"
#include "raylib.h"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720




int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sunshine");
    SetTargetFPS(60);

    rlImGuiSetup(true); // Setup GUI Init
    bool useGUI = false; // bool check to see if GUI is being used

    Vector2 position = { 0, 0 }; // starting position vector
    Vector2 velocity = { 2, 5 }; // starting velocity vector

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Kinematics Simulation!", 16, 9, 20, RED);

        float deltaTime = GetFrameTime(); // Get time in seconds for last frame drawn (delta time)

        Vector2 acceleration; // acceleration of object
    	float maxSpeed = 300.0f; // minimum to maximum speed setting changer for GUI interface

        DrawRectangle(970, 10, 300, 30, { 200, 150, 200, 255 });
        DrawText("Press ~ to open/close GUI", 980, 15, 20, WHITE); // GUI Instructions

        if (IsKeyPressed(KEY_GRAVE))
        {
            useGUI = !useGUI; // Flip trigger for GUI Render when Tilde (~) Key is pressed
        }

        if (useGUI) // Gui window Open
        {
            rlImGuiBegin();

            ImGui::SliderFloat("Velocity X", &velocity.x, -maxSpeed, maxSpeed, "%.3f", 0); // Slider for the X position velocity speed setting of the Object
            ImGui::SliderFloat("Velocity Y", &velocity.y, -maxSpeed, maxSpeed,"%.3f", 0); // Slider for the Y position velocity speed setting of the Object
            
            rlImGuiEnd();
        }

        position = Vector2Add(position, Vector2Scale(velocity, deltaTime)); // update the object current position according to the velocity formula -> position = position(dx,dy) + velocity * deltatime;
        DrawCircleV(position, 40, PURPLE); // render the kinematic object

        if (position.x > SCREEN_WIDTH) // if the X position exceeds the maximum WIDTH
            position.x = 0; // Wrap around to other side of the width starting at X position 0
        else if (position.x < 0) // if the X position exceeds the minimum WIDTH
            position.x = SCREEN_WIDTH; // Wrap around to other side of the width starting at Maximum WIDTH

        if (position.y > SCREEN_HEIGHT) // if the Y position exceeds the maximum HEIGHT
            position.y = 0; // Wrap around to other side of the height starting at Y position 0
        else if (position.y < 0) // if the Y position exceeds the minimum HEIGHT
            position.y = SCREEN_HEIGHT; // Wrap around to other side of the height starting at Maximum HEIGHT
        
        std::cout << "Position: (" << position.x << ", " << position.y << ")" << std::endl; // Console output to check position of Object
        std::cout << "Time: " << deltaTime << std::endl; // Console output to ensure delta time is functioning correctly according to frames

        EndDrawing();
    }

    rlImGuiShutdown(); // shut down GUI
    CloseWindow();
    return 0;
}