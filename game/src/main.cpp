#include <iostream>
#include "rlImGui.h"
#include "raymath.h"
#include "raylib.h"
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

//std::vector<Agent> agents; // container of agents


int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sunshine"); // Create Window
    SetTargetFPS(60); // Maximum Target FPS

    rlImGuiSetup(true); // Setup GUI Init
    bool useGUI = false; // bool check to see if GUI is being used

    // Old Way of creating a Vector2 Agent object that tracks or flees from a Vector2 mouse-controlled Target.
    /*
    Vector2 position = { 0, 0 }; // starting position vector
    Vector2 velocity = { 2, 5 }; // starting velocity vector before gravity factor
    Vector2 velocity = { 0, 0 }; // starting velocity vector for Agent to Target behaviour
    Vector2 acceleration = {0.0f, 9.8f}; // acceleration of object vector with gravity factor
    Vector2 acceleration = {0.0f, 0.0f}; // starting acceleration of object vector for Agent to Target Behaviour
    Vector2 displacement; // Object displacement vector
    Vector2 scaledAcceleration; // Acceleration increase over time vector

    Vector2 targetPosition; // Agent target position for mouse

    float maxAcceleration = 0.0f; // maximum acceleration speed when Agent is tracking target
    */

    Agent agent; // agent object
    agent.rigidBody.position = { 0, 0 }; // agent starting position
    agent.rigidBody.velocity = { 0,0 }; // agent starting velocity
    // Image Source Download Page: https://pngtree.com/freepng/blue-bird-flying-bird-cartoon-blue_3926050.html
    agent.sprite = LoadTexture("../game/assets/textures/agent_sprite.png"); // agent sprite
    agent.maxSpeed = 300.0f; // agent starting speed
    agent.maxAcceleration = 300.0f; // agent starting acceleration
    
    // Not very functional attempt at vector container of agent objects
    //agents.push_back(Agent());
    //agents.push_back(Agent());
    //agents.push_back(Agent());
    //agents.push_back(Agent());
    //agents.push_back(Agent());
    //agents.push_back(Agent());

    //for (Agent& currentAgent : agents)
    //{
    //    currentAgent.rigidBody.position = { 0, 0 };
    //    currentAgent.rigidBody.velocity = { 0, 0 };
    //    currentAgent.sprite = LoadTexture("../game/assets/textures/agent_sprite.png");
    //    currentAgent.maxAcceleration = 300.0f;
    //    currentAgent.maxSpeed = currentAgent.maxAcceleration;
    //}

    Vector2 targetPosition; // Create a vector that will be used as the XY position of the Target
    bool isMouseButtonLeft = false; // keep track of the left mouse button being pressed/held
    bool isMouseButtonRight = false; // keep track of the right mouse button being pressed/held

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Kinematics Simulation!", 16, 9, 20, RED);

        float deltaTime = GetFrameTime(); // Get time in seconds for last frame drawn (delta time)

        // Old speed setting - Lab_2 Parts 1-12
    	//float maxSpeed = 300.0f; // minimum to maximum speed setting changer for GUI interface

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

        	//ImGui::SliderFloat("Velocity X", &velocity.x, -maxSpeed, maxSpeed, "%.3f", 0); // Slider for the X position velocity speed setting of the Object
            //ImGui::SliderFloat("Velocity Y", &velocity.y, -maxSpeed, maxSpeed,"%.3f", 0); // Slider for the Y position velocity speed setting of the Object
            //ImGui::SliderFloat("Max Seek Acceleration", &maxAcceleration, 0, maxSpeed,"%.3f", 0); // Slider for the Agent to Target Acceleration speed setting
            
            rlImGuiEnd(); // End GUI
        }

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

        std::cout << "--------------------------------------------------------" << std::endl; // Console outputs
        std::cout << "Agent Position: ( X: " << agent.rigidBody.position.x << ", Y: " << agent.rigidBody.position.y << ")" << std::endl; // Console output to check position of Agent Object
        std::cout << "Target Position: ( X: " << targetPosition.x << ", Y: " << targetPosition.y << ")" << std::endl; // Console output to check position of Target
        std::cout << "Frame DeltaTime: " << deltaTime << std::endl; // Console output to ensure delta time is functioning correctly according to frames
        std::cout << "Agent Velocity: ( X: " << agent.rigidBody.velocity.x << ", Y: " << agent.rigidBody.velocity.y << ")" << std::endl; // agent velocity output
        std::cout << "Displacement due to velocity: ( X: " << displacement.x << ", Y: " << displacement.y << ")" << std::endl; // per frame displacement
        std::cout << "Agent Acceleration: " << agent.maxAcceleration << std::endl; // agent acceleration output
        std::cout << "Distance from agent to target: " << distance << std::endl; // agent distance to target

        // Not very functional attempt at updating each agent in a created vector of agents
        //for (Agent& currentAgent : agents)
        //{
        //    if (isMouseButtonLeft)
        //    {
        //        currentAgent.seekUpdate(deltaTime, targetPosition);
        //        currentAgent.wraparound();
        //    }
        //    if (isMouseButtonRight)
        //    {
        //        currentAgent.fleeUpdate(deltaTime, targetPosition);
        //        currentAgent.wraparound();
        //    }

        //    DrawTexture(currentAgent.sprite, currentAgent.rigidBody.position.x, currentAgent.rigidBody.position.y, WHITE);

        //    //Vector2 agentVelocity = Vector2Normalize(agent.rigidBody.velocity);
        //    //DrawLineEx(agent.rigidBody.position, Vector2Add(agent.rigidBody.position, Vector2Scale(agentVelocity, 30)), 2, GREEN);

        //    //Vector2 agentAcceleration = { agent.maxAcceleration, agent.maxAcceleration };
        //    //DrawLineEx(agent.rigidBody.position, Vector2Add(agent.rigidBody.position, Vector2Scale(agentAcceleration, 0.1f)), 2, { 0,0,255,255 });

        //    //Vector2 toTarget = Vector2Normalize(Vector2Subtract(targetPosition, agent.rigidBody.position));
        //    //DrawLineEx(agent.rigidBody.position, Vector2Add(agent.rigidBody.position, Vector2Scale(toTarget, 30)), 2, RED);
        //}


        // Old Way of setting up and executing the created Vector2 Agent object that tracks or flees from a Vector2 mouse-controlled Target.
        /*
        velocity = Vector2Add(velocity, Vector2Scale(acceleration, deltaTime)); // update velocity using acceleration -> velocity = velocity + acceleration * deltatime
        displacement = Vector2Scale(velocity, deltaTime); // update displacement of object due to accelerated velocity -> displacement = velocity * deltatime 
        scaledAcceleration = Vector2Scale(acceleration, 0.5f * deltaTime * deltaTime); // update acceleration over time -> scaledAcceleration = acceleration * 0.5f * deltatime * deltatime
        // Next Code Line - Old Agent Position Update method
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
        // Next 8 Code Lines - Old Agent velocity and Acceleration calculation formula's 
        //Vector2 newAcceleration = Vector2Scale(direction, maxAcceleration); // calculate the acceleration of the vector based on formula -> newAcceleration = direction * maxAcceleration
        //velocity = Vector2Add(velocity, Vector2Scale(newAcceleration, deltaTime)); // calculate the new velocity based on formula -> velocity = velocity + (newAcceleratiton * deltatime)

        //float currentSpeed = Vector2Length(velocity); // Calculate the magnitude of the velocity vector
        //if (currentSpeed > maxAcceleration) // if current speed of agent is above the highest set maxAcceleration speed
        //{
		//	velocity = Vector2Scale(velocity, maxAcceleration / currentSpeed); // Scale the velocity vector down to the maxAcceleration speed with formula -> velocity * maxAcceleration / currentSpeed
        //}

        Vector2 desiredVel = Vector2Scale(direction, maxAcceleration);
        Vector2 deltaVel = Vector2Subtract(desiredVel, velocity); // Head to target, Seek mechanic
        Vector2 accel = Vector2Scale(Vector2Normalize(deltaVel), maxAcceleration);
        velocity = Vector2Add(velocity, Vector2Scale(accel, deltaTime));

        Vector2 toTarget = Vector2Normalize(Vector2Subtract(targetPosition, agentPosition)); // Normalized Vector that keeps track of the targetPosition according to sqrt((v.x*v.x) + (v.y*v.y))
        DrawLineEx(position, Vector2Add(position, Vector2Scale(toTarget, 20)), 2, ORANGE); // Draw a vector line displaying acceleration magnitude and direction towards target

        Vector2 desiredFlee = Vector2Scale(direction, maxAcceleration);
        Vector2 deltaFlee = Vector2Subtract(velocity, desiredFlee); // Run away from target, Flee mechanic
        Vector2 accelFlee = Vector2Scale(Vector2Normalize(deltaFlee), maxAcceleration);
        velocity = Vector2Add(velocity, Vector2Scale(accelFlee, deltaTime));

        Vector2 fleeTarget = Vector2Normalize(Vector2Subtract(targetPosition, agentPosition)); // Normalized Vector that keeps track of the targetPosition according to sqrt((v.x*v.x) + (v.y*v.y))
        DrawLineEx(position, Vector2Add(position, Vector2Scale(fleeTarget, 20)), 2, YELLOW); // Draw a vector line displaying acceleration magnitude and direction away from target

        float currentSpeed = Vector2Length(velocity);
        if (currentSpeed > maxAcceleration)
        {
            velocity = Vector2Scale(velocity, maxAcceleration / currentSpeed);
        }

        // Next Code Line - Current New Agent Position Update method
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
        std::cout << "Agent Position: ( X: " << position.x << ", Y: " << position.y << ")" << std::endl; // Console output to check position of Agent Object
        std::cout << "Target Position: ( X: " << targetPosition.x << ", Y: " << targetPosition.y << ")" << std::endl; // Console output to check position of Target
        std::cout << "Frame DeltaTime: " << deltaTime << std::endl; // Console output to ensure delta time is functioning correctly according to frames
        std::cout << "Displacement due to velocity: " << displacement.x << ", " << displacement.y << std::endl; // per frame displacement
        std::cout << "Acceleration over time: " << velocity.x << ", " << velocity.y << std::endl; // per frame acceleration
        std::cout << "Distance from agent to target: " << distance << std::endl; // agent distance to target
        */

        EndDrawing();
    }

    UnloadTexture(agent.sprite); // Unload the sprite to clean up memory
    rlImGuiShutdown(); // shut down GUI
    CloseWindow();
    return 0;
}