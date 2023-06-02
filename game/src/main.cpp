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
    //Vector2 velocity = { 2, 5 }; // starting velocity vector
    Vector2 velocity = { 0, 0 }; // starting velocity vector for Agent to Target behaviour
    //Vector2 acceleration = {0.0f, 9.8f}; // acceleration of object vector
    Vector2 acceleration = {0.0f, 0.0f}; // starting acceleration of object vector for Agent to Target Behaviour
    Vector2 displacement; // Object displacement vector
    Vector2 scaledAcceleration; // Acceleration increase over time vector

    Vector2 targetPosition; // Agent target position for mouse

    float maxAcceleration = 0.0f; // maximum acceleration speed when Agent is tracking target

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Kinematics Simulation!", 16, 9, 20, RED);

        float deltaTime = GetFrameTime(); // Get time in seconds for last frame drawn (delta time)

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
            ImGui::SliderFloat("Max Acceleration", &maxAcceleration, 0, maxSpeed,"%.3f", 0); // Slider for the Agent to Target Acceleration speed setting
            
            rlImGuiEnd();
        }

        velocity = Vector2Add(velocity, Vector2Scale(acceleration, deltaTime)); // update velocity using acceleration -> velocity = velocity + acceleration * deltatime
        displacement = Vector2Scale(velocity, deltaTime); // update displacement of object due to accelerated velocity -> displacement = velocity * deltatime 
        scaledAcceleration = Vector2Scale(acceleration, 0.5f * deltaTime * deltaTime); // update acceleration over time -> scaledAcceleration = acceleration * 0.5f * deltatime * deltatime
        position = Vector2Add(position, Vector2Add(displacement, scaledAcceleration)); // finally Update current position of object in space based on all above -> position = position + (displacement + scaledAcceleration)
        // putting all the above together -> position = position + displacement (velocity * delta time) + scaledAcceleration (acceleration * 0.5 * deltatime * deltatime)
        // this is the mathematical formula of -> p2 = p1 + v * dt + 0.5 * a * dt * dt; 

        DrawCircleV(position, 40, BLUE); // render the kinematic object after applying all calculations

        DrawLineEx(position, Vector2Add(position, Vector2Scale(velocity, 1)),2, RED); // Draw a vector line displaying velocity magnitude and direction (can change based on XY velocity)
        DrawLineEx(position, Vector2Add(position, Vector2Scale(acceleration, 5)),2, GREEN); // Draw a vector line displaying acceleration magnitude and direction (gravity so always down)

        targetPosition = GetMousePosition(); // get target position based on mouse XY position
        DrawCircleV(targetPosition, 10, {255,0,255,128}); // Draw a transparent circle to represent the mouse target

        Vector2 agentPosition = position; // get current Agent position (the moving object)

        Vector2 direction = Vector2Subtract(targetPosition, agentPosition); // subtract agentPosition from targetPosition according to formula -> ( v1.x - v2.x, v1.y - v2.y )
        // mathematical formula of this is -> ||v|| = sqrt(|v.x|^2 + |v.y|^2)
        direction = Vector2Normalize(direction); // normalize the direction vector according to formula -> sqrt((v.x*v.x) + (v.y*v.y))
        // mathematical formula of this is -> v(norm) = v/||v||, v != 0 where vector is not 0

        float distance = Vector2Distance(agentPosition, targetPosition); // calculate the distance from agent to target (mouse) according to formula -> sqrt((v1.x - v2.x)*(v1.x - v2.x) + (v1.y - v2.y)*(v1.y - v2.y));
        // mathematical formula for distance -> d = sqrt((v2.x - v1.x)^2 + (v2.y - v1.y)^2)

        Vector2 newAcceleration = Vector2Scale(direction, maxAcceleration); // calculate the acceleration of the vector based on formula -> newAcceleration = direction * maxAcceleratiton
        velocity = Vector2Add(velocity, Vector2Scale(newAcceleration, deltaTime)); // calculate the new velocity based on formula -> velocity = velocity + (newAcceleratiton * deltatime)

        float currentSpeed = Vector2Length(velocity); // Calculate the magnitude of the velocity vector
        if (currentSpeed > maxAcceleration) // if current speed of agent is above the highest set maxAcceleration speed
        {
            velocity = Vector2Scale(velocity, maxAcceleration / currentSpeed); // Scale the velocity vector down to the maxAcceleration speed with formula -> velocity * maxAcceleration / currentSpeed
        }

        position = Vector2Add(position, Vector2Scale(velocity, deltaTime)); // Update position of Agent object according to velocity and deltatime -> position = position + (velocity * deltatime)

        if (position.x > SCREEN_WIDTH) // if the X position exceeds the maximum WIDTH
            position.x = 0; // Wrap around to other side of the width starting at X position 0
        else if (position.x < 0) // if the X position exceeds the minimum WIDTH
            position.x = SCREEN_WIDTH; // Wrap around to other side of the width starting at Maximum WIDTH

        if (position.y > SCREEN_HEIGHT) // if the Y position exceeds the maximum HEIGHT
            position.y = 0; // Wrap around to other side of the height starting at Y position 0
        else if (position.y < 0) // if the Y position exceeds the minimum HEIGHT
            position.y = SCREEN_HEIGHT; // Wrap around to other side of the height starting at Maximum HEIGHT

        std::cout << "--------------------------------------------------------" << std::endl;
        std::cout << "Position: ( X: " << position.x << ", Y: " << position.y << ")" << std::endl; // Console output to check position of Object
        std::cout << "Frame DeltaTime: " << deltaTime << std::endl; // Console output to ensure delta time is functioning correctly according to frames
        std::cout << "Displacement due to velocity: " << displacement.x << ", " << displacement.y << std::endl; // per frame displacement
        std::cout << "Acceleration over time: " << scaledAcceleration.x << ", " << scaledAcceleration.y << std::endl; // per frame acceleration
        std::cout << "Distance from agent to target: " << distance << std::endl; // agent distance to target

        EndDrawing();
    }

    rlImGuiShutdown(); // shut down GUI
    CloseWindow();
    return 0;
}