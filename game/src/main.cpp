#include <iostream>
#include "rlImGui.h"
#include "raymath.h"
#include "raylib.h"
//#include "Math.h"
#include <vector>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

class Rigidbody
{
public:
    Vector2 position; // position vector
    Vector2 velocity; // velocity vector
    Vector2 acceleration;
    Vector2 direction;
    float angularSpeed;

    void Update(float deltaTime)
	{
        position = Vector2Add(position, Vector2Scale(velocity, deltaTime)); // Update the position of the object(s)
    }
};

class Agent
{
public:
    Rigidbody rigidBody; // rigidbody object
    Texture2D sprite; // sprite for Agent
    float maxSpeed; // maxSpeed variable
    float maxAcceleration; // MaxAcceleration variable

    void seekUpdate(float deltaTime, const Vector2& targetPosition)
	{
    	Vector2 acceleration = Seek(targetPosition); // create an acceleration vector and call the Seek Function with the target position
        rigidBody.velocity = Vector2Add(rigidBody.velocity, Vector2Scale(acceleration, deltaTime)); // add acceleration to the current agent velocity towards target
        LimitSpeed(); // call limitSpeed function

        rigidBody.Update(deltaTime); // update Agent rigidbody position based on deltatime
    }

    void fleeUpdate(float deltaTime, const Vector2& targetPosition)
    {
        Vector2 acceleration = Flee(targetPosition); // create an acceleration vector and call the Flee Function with the target position
        rigidBody.velocity = Vector2Subtract(rigidBody.velocity, Vector2Scale(acceleration, deltaTime)); // add negative acceleration to the current agent velocity away from target
        LimitSpeed();// call limitSpeed function

        rigidBody.Update(deltaTime); // update Agent rigidbody position based on deltatime
    }

    Vector2 Seek(const Vector2& targetPosition)
	{
        Vector2 direction = Vector2Normalize(Vector2Subtract(targetPosition, rigidBody.position)); // create a direction vector and subtract the agent position from the target position -> result = { v1.x - v2.x, v1.y - v2.y };
        Vector2 desiredVelocity = Vector2Scale(direction, maxSpeed); // create a desiredVelocity vector that is a scaled vector based on direction and maxSpeed -> result = { v.x*scale, v.y*scale };
        Vector2 deltaVelocity = Vector2Subtract(desiredVelocity, rigidBody.velocity); // create a deltaVelocity vector and subtract the current agent velocity from the desired velocity -> result = { v1.x - v2.x, v1.y - v2.y };
        return Vector2Scale(Vector2Normalize(deltaVelocity), maxAcceleration); // return a scaled vector of a normalized deltaVelocity multiplied by the maxAcceleration -> result = { v.x*scale, v.y*scale }; sqrtf((v.x*v.x) + (v.y*v.y));
    }

    Vector2 Flee(const Vector2& targetPosition)
    {
        Vector2 direction = Vector2Normalize(Vector2Subtract(targetPosition, rigidBody.position)); // create a direction vector and subtract the agent position from the target position -> result = { v1.x - v2.x, v1.y - v2.y };
        Vector2 desiredVelocity = Vector2Scale(direction, maxSpeed); // create a desiredVelocity vector that is a scaled vector based on direction and maxSpeed -> result = { v.x*scale, v.y*scale };
        Vector2 deltaVelocity = Vector2Add(desiredVelocity, rigidBody.velocity); // create a deltaVelocity vector and add desired velocity to the current agent velocity to achieve further distance away from target -> result = { v1.x + v2.x, v1.y + v2.y };
        return Vector2Scale(Vector2Normalize(deltaVelocity), maxAcceleration); // return a scaled vector of a normalized deltaVelocity multiplied by the maxAcceleration -> result = { v.x*scale, v.y*scale }; sqrtf((v.x*v.x) + (v.y*v.y));
    }

    void LimitSpeed()
	{
        float currentSpeed = Vector2Length(rigidBody.velocity); // calculate the current speed of the agent rigidbody using Vector2Length formula -> result = sqrtf((v.x*v.x) + (v.y*v.y));
        if (currentSpeed > maxSpeed) // If current speed exceeds the maximum speed set
        {
            // Reduce velocity and maintain vector direction
        	rigidBody.velocity = Vector2Scale(rigidBody.velocity, maxSpeed / currentSpeed); // adjust the vector of the rigidbody by multiplying velocity with maxSpeed/currentSpeed -> result = { v.x*scale, v.y*scale };
        }
    }

    void wraparound()
    {
        if (rigidBody.position.x > SCREEN_WIDTH) // if the X position exceeds the maximum WIDTH
            rigidBody.position.x = 0; // Wrap around to other side of the width starting at X position 0
        else if (rigidBody.position.x < 0) // if the X position exceeds the minimum WIDTH
            rigidBody.position.x = SCREEN_WIDTH; // Wrap around to other side of the width starting at Maximum WIDTH

        if (rigidBody.position.y > SCREEN_HEIGHT) // if the Y position exceeds the maximum HEIGHT
            rigidBody.position.y = 0; // Wrap around to other side of the height starting at Y position 0
        else if (rigidBody.position.y < 0) // if the Y position exceeds the minimum HEIGHT
            rigidBody.position.y = SCREEN_HEIGHT; // Wrap around to other side of the height starting at Maximum HEIGHT
    }
};

// Function to calculate the perpendicular distance between a point and a line
float PointToLineDistance(Vector2 point, Vector2 lineStart, Vector2 lineEnd)
{
    float lineLength = Vector2Distance(lineStart, lineEnd);

    // If the line has zero length, return the distance between the point and the line start
    if (lineLength == 0.0f)
        return Vector2Distance(point, lineStart);

    // Calculate the normalized direction vector of the line
    Vector2 lineDirection = Vector2Normalize(Vector2Subtract(lineEnd, lineStart));

    // Calculate the vector from the line start to the point
    Vector2 pointToLineStart = Vector2Subtract(point, lineStart);

    // Calculate the projection of the point onto the line
    float projection = Vector2DotProduct(pointToLineStart, lineDirection);

    // Clamp the projection value to the valid range [0, lineLength]
    projection = Clamp(projection, 0.0f, lineLength);

    // Calculate the nearest point on the line to the point
    Vector2 nearestPoint = Vector2Add(lineStart, Vector2Scale(lineDirection, projection));

    // Calculate the distance between the point and the nearest point on the line
    return Vector2Distance(point, nearestPoint);
}


int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sunshine"); // Create Window
    SetTargetFPS(60); // Maximum Target FPS

    rlImGuiSetup(true); // Setup GUI Init
    bool useGUI = false; // bool check to see if GUI is being used

    Agent agent; // agent object
    agent.rigidBody.position = { 0, 0 }; // agent starting position
    agent.rigidBody.velocity = { 0,0 }; // agent starting velocity
    // Image Source Download Page: https://pngtree.com/freepng/blue-bird-flying-bird-cartoon-blue_3926050.html
    agent.sprite = LoadTexture("../game/assets/textures/agent_sprite.png"); // agent sprite
    agent.maxSpeed = 300.0f; // agent starting speed
    agent.maxAcceleration = 300.0f; // agent starting acceleration

    Vector2 targetPosition; // Create a vector that will be used as the XY position of the Target
    bool isMouseButtonLeft = false; // keep track of the left mouse button being pressed/held
    bool isMouseButtonRight = false; // keep track of the right mouse button being pressed/held

    float rotationAngle = 0.0f; // rotation for blue agent circle
    float maxSpeed = 300.0f;
    
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Whisker Sensors!", 16, 9, 20, RED);

        float deltaTime = GetFrameTime(); // Get time in seconds for last frame drawn (delta time)

        Vector2 agentPosition = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 }; // Agent starting position set to half the screen width and height
        //Vector2 agentVelocity = { 5.0f, 5.0f }; // velocity for the agent to work with
        Vector2 mousePosition = GetMousePosition(); // get mouse position vector based on XY coordinates in screen
        Vector2 obstaclePosition = {200, 200}; // get Stationary Obstacle vector based on XY coordinates in screen

        Vector2 direction = Vector2Subtract(mousePosition, agent.rigidBody.position); // Create a direction vector from the agent position to the obstacle position -> result = { v1.x - v2.x, v1.y - v2.y };

        DrawCircleV(obstaclePosition, 10, { 255,0,255,128 }); // Draw a transparent circle to represent the Stationary obstacle target
        DrawCircleV(mousePosition, 10, { 255,0,255,128 }); // Draw a transparent circle to represent the mouse obstacle target

        float lineLength = 100.0f; // Length of the whisker line

        // Create a vector endpoint which represents the agent position as a scaled normalized direction opposite from the mouse target with a set length
        Vector2 endPoint = Vector2Subtract(agent.rigidBody.position, Vector2Scale(Vector2Normalize(direction), lineLength)); // Mathematical formula -> result = { v1.x - v2.x, v1.y - v2.y }; result = { v.x*scale, v.y*scale };

        DrawLineEx(agent.rigidBody.position, endPoint, 1.5f,RED); // Draw the endPoint whisker line from the agent position away from the mouse target

        //DrawCircleV(agent.rigidBody.position, 10, BLUE); // Draw the circle representing the agent

        float whiskerAngle = 15.0f; // Angle of the whiskers in float degrees

        // create a vector which originates from the direction vector and is based on the whiskerAngle
        // Mathematical Formula -> cosres = cosf(angle); sinres = sinf(angle);
        // new angle vector x = (input vector x * cos angle) - (input vector y * sin angle)
        // new angle vector y = (input vector x * sin angle) + (input vector y * cos angle)
        // return the new angle vector(x,y)
        Vector2 rightWhiskerDirection = Vector2Rotate(direction, whiskerAngle * DEG2RAD); // clockwise rotation -> result.x = v.x*cosres - v.y*sinres; result.y = v.x*sinres + v.y*cosres;
        Vector2 leftWhiskerDirection = Vector2Rotate(direction, -whiskerAngle * DEG2RAD); // counter-clockwise rotation -> result.x = v.x*(-cosres) - v.y*(-sinres); result.y = v.x*(-sinres) + v.y*(-cosres);

        // create a rightWhiskerEnd vector starting from the agent.rigidBody.position and extending in the Scaled Normalized rightWhiskerDirection according to the lineLength
        Vector2 rightWhiskerEnd = Vector2Add(agent.rigidBody.position, Vector2Scale(Vector2Normalize(rightWhiskerDirection), lineLength)); // Mathematical understanding -> agent.rigidBody.position + (Scale Normalize (rightWhiskerDirection * LineLength)
        // create a leftWhiskerEnd vector starting from the agent.rigidBody.position and extending in the Scaled Normalized leftWhiskerDirection according to the lineLength
        Vector2 leftWhiskerEnd = Vector2Add(agent.rigidBody.position, Vector2Scale(Vector2Normalize(leftWhiskerDirection), lineLength)); // Mathematical understanding -> agent.rigidBody.position + (Scale Normalize (leftWhiskerDirection * LineLength)

        DrawLineEx(agent.rigidBody.position, rightWhiskerEnd, 1.5f, BLUE);  // Draw the right whisker line vector

        DrawLineEx(agent.rigidBody.position, leftWhiskerEnd, 1.5f, BLUE); // Draw the left whisker line vector

        Vector2 mouseCenter = mousePosition; // mouse obstacle vector
        float mouseRadius = 10.0f; // mouse obstacle radius

        float mouseRight = PointToLineDistance(mouseCenter, agent.rigidBody.position, rightWhiskerEnd);
        float mouseLeft = PointToLineDistance(mouseCenter, agent.rigidBody.position, leftWhiskerEnd);
        bool rightMouseLineCircleCollision = (mouseRight <= mouseRadius);
        bool LeftMouseLineCircleCollision = (mouseLeft <= mouseRadius);

        Vector2 ObstacleCenter = obstaclePosition; // stationary obstacle vector
        float ObstacleRadius = 10.0f; // stationary obstacle radius

        float ObstacleMouseRight = PointToLineDistance(ObstacleCenter, agent.rigidBody.position, rightWhiskerEnd);
        float ObstacleMouseLeft = PointToLineDistance(ObstacleCenter, agent.rigidBody.position, leftWhiskerEnd);
        bool RightObstacleCollision = (ObstacleMouseRight <= ObstacleRadius);
        bool LeftObstacleCollision = (ObstacleMouseLeft <= ObstacleRadius);

        // Lab 3 - Part 2
        //Vector2 nearestPoint = Vector2Add(agent.rigidBody.position, Vector2Scale(Vector2Normalize(direction), lineLength));
        //float distance = Vector2Distance(nearestPoint, circleCenter);
        //bool lineCircleCollision = (distance < circleRadius);

        float cosAngle = cosf(rotationAngle * DEG2RAD); // Calculate cosine of the angle
        float sinAngle = sinf(rotationAngle * DEG2RAD); // Calculate sine of the angle

        DrawCircle(agent.rigidBody.position.x, agent.rigidBody.position.y, 10, BLUE);
        //DrawCircle(agent.rigidBody.position.x + 20.0f * cosAngle, agent.rigidBody.position.y + 20.0f * sinAngle, 10, BLUE);

        targetPosition = GetMousePosition(); // use mouse position for the target XY position

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) // if left mouse button is being held
        {
            isMouseButtonLeft = true;
            DrawCircleV(targetPosition, 10, { 255,0,255,128 }); // Draw a transparent circle to represent the mouse target
        }
        else
        {
            isMouseButtonLeft = false;
        }

        if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) // if right mouse button is being held
        {
            isMouseButtonRight = true;
            DrawCircleV(targetPosition, 10, { 255,0,255,128 }); // Draw a transparent circle to represent the mouse target
        }
        else
        {
            isMouseButtonRight = false;
        }

        if (isMouseButtonLeft)
        {
            agent.seekUpdate(deltaTime, targetPosition); // Call Seek function for left mouse button being held
            agent.wraparound(); // make sure the objects wrap around to the other side instead of disappear off-screen
        }
        if (isMouseButtonRight)
        {
            agent.fleeUpdate(deltaTime, targetPosition); // Call Flee function for right mouse button being held
            agent.wraparound(); // make sure the objects wrap around to the other side instead of disappear off-screen
        }

        DrawTexture(agent.sprite, agent.rigidBody.position.x, agent.rigidBody.position.y, WHITE); // Render the agent sprite at its current XY position

        Vector2 agentVelocity = Vector2Normalize(agent.rigidBody.velocity); // Create a Normalized vector based on the velocity for the Agent
        DrawLineEx(agent.rigidBody.position, Vector2Add(agent.rigidBody.position, Vector2Scale(agentVelocity, 30)), 2, GREEN); // Draw a vector line displaying velocity magnitude and direction towards target

        Vector2 agentAcceleration = { agent.maxAcceleration, agent.maxAcceleration }; // Create a vector attached to the current XY for the Agent
        DrawLineEx(agent.rigidBody.position, Vector2Add(agent.rigidBody.position, Vector2Scale(agentAcceleration, 0.1f)), 2, { 0,0,255,255 }); // Draw a vector line for the acceleration magnitude of the Agent

        Vector2 toTarget = Vector2Normalize(Vector2Subtract(targetPosition, agent.rigidBody.position)); // Normalized Vector that keeps track of the targetPosition with respect to Agent position according to sqrt((v.x*v.x) + (v.y*v.y))
        DrawLineEx(agent.rigidBody.position, Vector2Add(agent.rigidBody.position, Vector2Scale(toTarget, 30)), 2, RED); // Draw a vector line displaying the direction towards target from agent

        Vector2 displacement = Vector2Scale(agent.rigidBody.velocity, deltaTime); // update displacement of object due to accelerated velocity -> displacement = velocity * deltatime 
        float distance = Vector2Distance(agent.rigidBody.position, targetPosition); // calculate the distance from agent to target (mouse) according to formula -> sqrt((v1.x - v2.x)*(v1.x - v2.x) + (v1.y - v2.y)*(v1.y - v2.y));

        if (rightMouseLineCircleCollision) // turn to the left
        {
            DrawCircleV(mouseCenter, 10, GREEN); // collision color swap for target
            DrawCircleV(agent.rigidBody.position, 10, GREEN); // collision color swap for agent
            rotationAngle -= 1.0f; // Rotate counter-clockwise
            agentVelocity.x = sinAngle;
            agentVelocity.y = cosAngle;
            DrawCircle(agent.rigidBody.position.x + 20.0f * sinAngle, agent.rigidBody.position.y + 20.0f * cosAngle, 10, GREEN);
            std::cout << "Line-Circle Collision!" << std::endl; 
        }

        if (LeftMouseLineCircleCollision) // turn to the right
        {
            DrawCircleV(mouseCenter, 10, GREEN); // collision color swap for target
            DrawCircleV(agent.rigidBody.position, 10, GREEN); // collision color swap for agent
            rotationAngle -= 1.0f; // Rotate counter-clockwise
            agentVelocity.x = -cosAngle;
            agentVelocity.y = -sinAngle;
            DrawCircle(agent.rigidBody.position.x - 20.0f * cosAngle, agent.rigidBody.position.y - 20.0f * sinAngle, 10, GREEN);
            std::cout << "Line-Circle Collision!" << std::endl;
        }

        if (RightObstacleCollision)
        {
            std::cout << "imminent collision with stationary obstacle and right whisker!" << std::endl;
            Vector2 linearDirection = Vector2Normalize(agent.rigidBody.velocity);
            float linearSpeed = Vector2Length(agent.rigidBody.velocity);
            agent.rigidBody.velocity = Vector2Scale(Vector2Rotate(linearDirection, -agent.rigidBody.angularSpeed * deltaTime * DEG2RAD), linearSpeed);
        }

        if (LeftObstacleCollision)
        {
            std::cout << "imminent collision with stationary obstacle and left whisker!" << std::endl;
            Vector2 linearDirection = Vector2Normalize(agent.rigidBody.velocity);
            float linearSpeed = Vector2Length(agent.rigidBody.velocity);
            agent.rigidBody.velocity = Vector2Scale(Vector2Rotate(linearDirection, -agent.rigidBody.angularSpeed * deltaTime * DEG2RAD), linearSpeed);
        }

        DrawRectangle(970, 10, 300, 30, { 200, 150, 200, 255 });
        DrawText("Press ~ to open/close GUI", 980, 15, 20, WHITE); // GUI Instructions

        if (IsKeyPressed(KEY_GRAVE))
        {
            useGUI = !useGUI; // Flip trigger for GUI Render when Tilde (~) Key is pressed
        }

        if (useGUI) // GUI window Open
        {
            rlImGuiBegin(); // Start GUI

            ImGui::SliderFloat("Position X", &agent.rigidBody.position.x, 0, SCREEN_WIDTH, "%.3f", 0); // Slider for the X position of the Object(s)
            ImGui::SliderFloat("Position Y", &agent.rigidBody.position.y, 0, SCREEN_HEIGHT, "%.3f", 0); // Slider for the Y position of the Object(s)
            ImGui::SliderFloat("Velocity X", &agent.rigidBody.velocity.x, -agent.maxSpeed, agent.maxSpeed, "%.3f", 0); // Slider for the X Velocity of the Object(s)
            ImGui::SliderFloat("Velocity Y", &agent.rigidBody.velocity.y, -agent.maxSpeed, agent.maxSpeed, "%.3f", 0); // Slider for the Y Velocity of the Object(s)
            ImGui::SliderFloat("Max Speed", &agent.maxSpeed, 0.0f, 1000.0f, "%.3f", 0); // Slider for the maxSpeed of the Object(s)
            ImGui::SliderFloat("Max Acceleration", &agent.maxAcceleration, 0.0f, 1000.0f, "%.3f", 0); // Slider for the maxAcceleration of the Objects(s) - how fast the object(s) can reach the maxSpeed

        	ImGui::SliderFloat("Right Whisker X", &rightWhiskerEnd.x, 0, SCREEN_WIDTH, "%.3f", 0); // Slider for the X position of the Object(s)
            ImGui::SliderFloat("Right Whisker Y", &rightWhiskerEnd.y, 0, SCREEN_HEIGHT, "%.3f", 0); // Slider for the Y position of the Object(s)
            ImGui::SliderFloat("Left Whisker X", &leftWhiskerEnd.x, 0, SCREEN_WIDTH, "%.3f", 0); // Slider for the X position of the Object(s)
            ImGui::SliderFloat("Left Whisker Y", &leftWhiskerEnd.y, 0, SCREEN_HEIGHT, "%.3f", 0); // Slider for the Y position of the Object(s)
            ImGui::SliderFloat("Angular Velocity X", &agentVelocity.x, -maxSpeed, maxSpeed, "%.3f", 0); // Slider for the X Velocity of the Object(s)
            ImGui::SliderFloat("Angular Velocity Y", &agentVelocity.y, -maxSpeed, maxSpeed, "%.3f", 0); // Slider for the Y Velocity of the Object(s)

            rlImGuiEnd(); // End GUI
        }

        std::cout << "--------------------------------------------------------" << std::endl; // Console outputs
        std::cout << "Frame DeltaTime: " << deltaTime << std::endl; // Console output to ensure delta time is functioning correctly according to frames
    	std::cout << "Agent Position: ( X: " << agent.rigidBody.position.x << ", Y: " << agent.rigidBody.position.y << ")" << std::endl; // Console output to check position of Agent Object
        std::cout << "Target Position: ( X: " << targetPosition.x << ", Y: " << targetPosition.y << ")" << std::endl; // Console output to check position of Target
        std::cout << "Stationary Target Position: ( X: " << obstaclePosition.x << ", Y: " << obstaclePosition.y << ")" << std::endl; // Console output to check position of Target
        std::cout << "Agent Velocity: ( X: " << agent.rigidBody.velocity.x << ", Y: " << agent.rigidBody.velocity.y << ")" << std::endl; // agent velocity output
        std::cout << "Whisker Agent Velocity: ( X: " << agentVelocity.x << ", Y: " << agentVelocity.y << ")" << std::endl; // agent velocity output
        std::cout << "Displacement due to velocity: ( X: " << displacement.x << ", Y: " << displacement.y << ")" << std::endl; // per frame displacement
        std::cout << "Agent Acceleration: " << agent.maxAcceleration << std::endl; // agent acceleration output
        std::cout << "Distance from agent to target: " << distance << std::endl; // agent distance to target
        std::cout << "--------------------------------------------------------" << std::endl; // Console outputs

        EndDrawing();
    }
    CloseWindow();
    return 0;
}