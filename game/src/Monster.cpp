#include "Monster.h"

#include "raymath.h"

// Assignment 2 - Part 6
Monster::Monster(const char* texturePath, Vector2 startPos) : Character(texturePath, startPos) // extend character constructor
{
	sprite = LoadTexture(texturePath); // load texture using the provided texture path
	position = startPos; // set the position for monster according to current position
}

void Monster::Draw() // Draw Monster
{
	DrawTextureEx(sprite, position, 0.0f, 3.0f, WHITE); // render out the monster sprite at the current position
    DrawCircleV(position, 10, RED); // Debugging purposes, comment out when not needed
}

void Monster::SetGoal(TileCoordinates goal) // Set the monster goal tile and call the PathFinding to the monsterGoal
{
	monsterGoal = goal; // set the monsterGoal to the goal passed along as a TileCoordinate
	MonsterPath(); // call MonsterPath function
}

void Monster::SetTileSize(Vector2 tileSize) // Set the size of the tile for the monster PathFinding
{
	monsterTileSize = tileSize; // assign monster tileSize according to the tileSize being passed to the function
}

void Monster::MonsterPath() // Set minotaurStart at the current position for it and move towards the MonsterTileSize goal
{
	// PathFind from current monster position to the goal (Vector2 & Vector2 data type)
	TileCoordinates minotaurStart = TileCoordinates::ScreenToTileCoordinates(position, monsterTileSize);
}

void Monster::MonsterUpdate(float deltaTime)
{
    // check if there are any tiles left in the path
    if (!monsterPath.empty())
    {
        // move towards the next tile in the path at the monster's speed
        Vector2 goalPos = TileCoordinates::TileToScreenCoordinates(monsterPath.front(), monsterTileSize);
        Vector2 direction = Vector2Subtract(goalPos, position);
        if (Vector2Length(direction) > monsterSpeed * deltaTime)
        {
            // if the goal is further away than we can move this frame, normalize the direction
            direction = Vector2Normalize(direction);
        }
        position = Vector2Add(position, Vector2Scale(direction, monsterSpeed * deltaTime));

        // if we've reached the goal tile, remove it from the path
        if (Vector2Distance(position, goalPos) < monsterSpeed * deltaTime)
        {
            monsterPath.erase(monsterPath.begin());
        }
    }
}