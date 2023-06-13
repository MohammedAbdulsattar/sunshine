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

    float rotationAngle = 0.0f; // rotation for blue agent circle
    float maxSpeed = 300.0f;
    
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Whisker Sensors!", 16, 9, 20, RED);

        float deltaTime = GetFrameTime(); // Get time in seconds for last frame drawn (delta time)

        //Vector2 a = { 100,100 };
    	//Vector2 b = { 300,300 };
        //Vector2 c = {200,200};

        //Project(a, b);
        //Vector2 result = NearestPoint(a, b, c);

        Vector2 agentPosition = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 }; // Agent starting position set to half the screen width and height
        Vector2 agentVelocity = { 5.0f, 5.0f }; // velocity for the agent to work with
        Vector2 mousePosition = GetMousePosition(); // get mouse position vector based on XY coordinates in screen
        Vector2 obstaclePosition = {200, 200}; // get mouse position vector based on XY coordinates in screen

        Vector2 direction = Vector2Subtract(obstaclePosition, agentPosition); // Create a direction vector from the agent position to the obstacle position -> result = { v1.x - v2.x, v1.y - v2.y };

        DrawCircleV(obstaclePosition, 10, { 255,0,255,128 }); // Draw a transparent circle to represent the mouse obstacle target
        DrawCircleV(mousePosition, 10, { 255,0,255,128 }); // Draw a transparent circle to represent the mouse obstacle target

        float lineLength = 100.0f; // Length of the whisker line

        // Create a vector endpoint which represents the agent position as a scaled normalized direction opposite from the mouse target with a set length
        Vector2 endPoint = Vector2Subtract(agentPosition, Vector2Scale(Vector2Normalize(direction), lineLength)); // Mathematical formula -> result = { v1.x - v2.x, v1.y - v2.y }; result = { v.x*scale, v.y*scale };

        DrawLineEx(agentPosition, endPoint, 1.5f,RED); // Draw the endPoint whisker line from the agent position away from the mouse target

        //DrawCircleV(agentPosition, 10, BLUE); // Draw the circle representing the agent

        float whiskerAngle = 270.0f; // Angle of the whiskers in float degrees

        // create a vector which originates from the direction vector and is based on the whiskerAngle
        // Mathematical Formula -> cosres = cosf(angle); sinres = sinf(angle);
        // new angle vector x = (input vector x * cos angle) - (input vector y * sin angle)
        // new angle vector y = (input vector x * sin angle) + (input vector y * cos angle)
        // return the new angle vector(x,y)
        Vector2 rightWhiskerDirection = Vector2Rotate(direction, whiskerAngle); // clockwise rotation -> result.x = v.x*cosres - v.y*sinres; result.y = v.x*sinres + v.y*cosres;
        Vector2 leftWhiskerDirection = Vector2Rotate(direction, -whiskerAngle); // counter-clockwise rotation -> result.x = v.x*(-cosres) - v.y*(-sinres); result.y = v.x*(-sinres) + v.y*(-cosres);

        // create a rightWhiskerEnd vector starting from the agentPosition and extending in the Scaled Normalized rightWhiskerDirection according to the lineLength
        Vector2 rightWhiskerEnd = Vector2Add(agentPosition, Vector2Scale(Vector2Normalize(rightWhiskerDirection), lineLength)); // Mathematical understanding -> agentposition + (Scale Normalize (rightWhiskerDirection * LineLength)
        // create a leftWhiskerEnd vector starting from the agentPosition and extending in the Scaled Normalized leftWhiskerDirection according to the lineLength
        Vector2 leftWhiskerEnd = Vector2Add(agentPosition, Vector2Scale(Vector2Normalize(leftWhiskerDirection), lineLength)); // Mathematical understanding -> agentposition + (Scale Normalize (leftWhiskerDirection * LineLength)

        DrawLineEx(agentPosition, rightWhiskerEnd, 1.5f, BLUE);  // Draw the right whisker line vector

        DrawLineEx(agentPosition, leftWhiskerEnd, 1.5f, BLUE); // Draw the left whisker line vector

        //Vector2 circleCenter = obstaclePosition; // stationary obstacle vector
        //float circleRadius = 10.0f; // stationary obstacle radius

        //float distanceRight = PointToLineDistance(circleCenter, agentPosition, rightWhiskerEnd);
        //float distanceLeft = PointToLineDistance(circleCenter, agentPosition, leftWhiskerEnd);
        //bool RightLineCircleCollision = (distanceRight <= circleRadius);
        //bool leftLineCircleCollision = (distanceLeft <= circleRadius);

        Vector2 mouseCenter = mousePosition; // mouse obstacle vector
        float mouseRadius = 10.0f; // mouse obstacle radius

        float mouseRight = PointToLineDistance(mouseCenter, agentPosition, rightWhiskerEnd);
        float mouseLeft = PointToLineDistance(mouseCenter, agentPosition, leftWhiskerEnd);
        bool rightMouseLineCircleCollision = (mouseRight <= mouseRadius);
        bool LeftMouseLineCircleCollision = (mouseLeft <= mouseRadius);

        // Lab 3 - Part 2
        //Vector2 nearestPoint = Vector2Add(agentPosition, Vector2Scale(Vector2Normalize(direction), lineLength));
        //float distance = Vector2Distance(nearestPoint, circleCenter);
        //bool lineCircleCollision = (distance < circleRadius);

        agentPosition.x += agentVelocity.x * deltaTime;
        agentPosition.y += agentVelocity.y * deltaTime;

        float cosAngle = cosf(rotationAngle * DEG2RAD); // Calculate cosine of the angle
        float sinAngle = sinf(rotationAngle * DEG2RAD); // Calculate sine of the angle

        DrawCircle(agentPosition.x, agentPosition.y, 10, BLUE);
        //DrawCircle(agentPosition.x + 20.0f * cosAngle, agentPosition.y + 20.0f * sinAngle, 10, BLUE);

        //if (RightLineCircleCollision)
        //{
        //    DrawCircleV(circleCenter, 10, GREEN); // collision color swap for target
        //    DrawCircleV(agentPosition,10, GREEN); // collision color swap for agent
        //    rotationAngle += 1.0f; // Rotate clockwise
        //    DrawCircle(agentPosition.x + 20.0f * cosAngle, agentPosition.y + 20.0f * sinAngle, 10, BLUE);
        //	std::cout << "Line-Circle Collision!" << std::endl;
        //}

        //if (leftLineCircleCollision)
        //{
        //    DrawCircleV(circleCenter, 10, GREEN); // collision color swap for target
        //    DrawCircleV(agentPosition, 10, GREEN); // collision color swap for agent
        //    rotationAngle += 1.0f; // Rotate clockwise
        //    DrawCircle(agentPosition.x - 20.0f * cosAngle, agentPosition.y - 20.0f * sinAngle, 10, BLUE);
        //    std::cout << "Line-Circle Collision!" << std::endl;
        //}

        if (rightMouseLineCircleCollision) // turn to the left
        {
            DrawCircleV(mouseCenter, 10, GREEN); // collision color swap for target
            DrawCircleV(agentPosition, 10, GREEN); // collision color swap for agent
            rotationAngle -= 1.0f; // Rotate counter-clockwise
            agentVelocity.x = sinAngle;
            agentVelocity.y = cosAngle;
            DrawCircle(agentPosition.x + 20.0f * sinAngle, agentPosition.y + 20.0f * cosAngle, 10, GREEN);
            std::cout << "Line-Circle Collision!" << std::endl; 
        }

        if (LeftMouseLineCircleCollision) // turn to the right
        {
            DrawCircleV(mouseCenter, 10, GREEN); // collision color swap for target
            DrawCircleV(agentPosition, 10, GREEN); // collision color swap for agent
            rotationAngle -= 1.0f; // Rotate counter-clockwise
            agentVelocity.x = -cosAngle;
            agentVelocity.y = -sinAngle;
            DrawCircle(agentPosition.x - 20.0f * cosAngle, agentPosition.y - 20.0f * sinAngle, 10, GREEN);
            std::cout << "Line-Circle Collision!" << std::endl;
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

            ImGui::SliderFloat("Position X", &agentPosition.x, 0, SCREEN_WIDTH, "%.3f", 0); // Slider for the X position of the Object(s)
            ImGui::SliderFloat("Position Y", &agentPosition.y, 0, SCREEN_HEIGHT, "%.3f", 0); // Slider for the Y position of the Object(s)
            ImGui::SliderFloat("Velocity X", &agentVelocity.x, -maxSpeed, maxSpeed, "%.3f", 0); // Slider for the X Velocity of the Object(s)
            ImGui::SliderFloat("Velocity Y", &agentVelocity.y, -maxSpeed, maxSpeed, "%.3f", 0); // Slider for the Y Velocity of the Object(s)
            //ImGui::SliderFloat("Max Speed", &agent.maxSpeed, 0.0f, 1000.0f, "%.3f", 0); // Slider for the maxSpeed of the Object(s)
            //ImGui::SliderFloat("Max Acceleration", &agent.maxAcceleration, 0.0f, 1000.0f, "%.3f", 0); // Slider for the maxAcceleration of the Objects(s) - how fast the object(s) can reach the maxSpeed

            rlImGuiEnd(); // End GUI
        }

        std::cout << "--------------------------------------------------------" << std::endl; // Console outputs
        std::cout << "Frame DeltaTime: " << deltaTime << std::endl; // Console output to ensure delta time is functioning correctly according to frames
    	std::cout << "Agent Position: ( X: " << agentPosition.x << ", Y: " << agentPosition.y << ")" << std::endl; // Console output to check position of Agent Object
        std::cout << "Moving Target Position: ( X: " << mousePosition.x << ", Y: " << mousePosition.y << ")" << std::endl; // Console output to check position of Target
        std::cout << "Stationary Target Position: ( X: " << obstaclePosition.x << ", Y: " << obstaclePosition.y << ")" << std::endl; // Console output to check position of Target
        std::cout << "Agent Velocity: ( X: " << agentVelocity.x << ", Y: " << agentVelocity.y << ")" << std::endl; // agent velocity output
        //std::cout << "Displacement due to velocity: ( X: " << displacement.x << ", Y: " << displacement.y << ")" << std::endl; // per frame displacement
        //std::cout << "Agent Acceleration: " << agent.maxAcceleration << std::endl; // agent acceleration output
        //std::cout << "Distance from agent to target: " << distance << std::endl; // agent distance to target
        //std::cout << "Nearest point Vector: ( X: " << nearestPoint.x << ", Y: " << nearestPoint.y << ")" << std::endl; // nearest angle points vector

        //Vector2Distance(rightWhiskerEnd, obstaclePosition);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}