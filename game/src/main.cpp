#include <iostream>
#include "rlImGui.h"
#include "raymath.h"
#include "raylib.h"
#include <vector>


#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sunshine");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Whisker Sensors!", 16, 9, 20, RED);

        Vector2 agentPosition = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 }; // Agent starting position set to half the screen width and height
        Vector2 obstaclePosition = GetMousePosition(); // get mouse position vector based on XY coordinates in screen

        Vector2 direction = Vector2Subtract(obstaclePosition, agentPosition); // Create a direction vector from the agent position to the obstacle position -> result = { v1.x - v2.x, v1.y - v2.y };

        DrawCircleV(obstaclePosition, 10, { 255,0,255,128 }); // Draw a transparent circle to represent the mouse obstacle target

        float lineLength = 100.0f; // Length of the whisker line

        // Create a vector endpoint which represents the agent position as a scaled normalized direction opposite from the mouse target with a set length
        Vector2 endPoint = Vector2Subtract(agentPosition, Vector2Scale(Vector2Normalize(direction), lineLength)); // Mathematical formula -> result = { v1.x - v2.x, v1.y - v2.y }; result = { v.x*scale, v.y*scale };

        DrawLineEx(agentPosition, endPoint, 1.5f,RED); // Draw the endPoint whisker line from the agent position away from the mouse target

        DrawCircleV(agentPosition, 10, BLUE); // Draw the circle representing the agent

        float whiskerAngle = 270.0f; // Angle of the whiskers in float degrees

        Vector2 rightWhiskerDirection = Vector2Rotate(direction, whiskerAngle);
        Vector2 leftWhiskerDirection = Vector2Rotate(direction, -whiskerAngle);

        Vector2 rightWhiskerEnd = Vector2Add(agentPosition, Vector2Scale(Vector2Normalize(rightWhiskerDirection), lineLength));
        Vector2 leftWhiskerEnd = Vector2Add(agentPosition, Vector2Scale(Vector2Normalize(leftWhiskerDirection), lineLength));

        DrawLineEx(agentPosition, rightWhiskerEnd, 1.5f, BLUE);  // Draw the right whisker line

        DrawLineEx(agentPosition, leftWhiskerEnd, 1.5f, BLUE); // Draw the left whisker line

        EndDrawing();
    }

    CloseWindow();
    return 0;
}