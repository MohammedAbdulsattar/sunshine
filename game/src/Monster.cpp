#include "Monster.h"

#include "raymath.h"

// Assignment 2 - Part 6
Monster::Monster(const char* texturePath, Vector2 startPos, float speed, Tilemap& map)
	: Character(texturePath, startPos), monsterSpeed(speed), tilemap(map) // extend character constructor
{
	sprite = LoadTexture(texturePath); // load texture using the provided texture path
	position = startPos; // set the position for monster according to current position
}

void Monster::Draw() // Draw Monster
{
	DrawTextureEx(sprite, position, 0.0f, 3.0f, WHITE); // render out the monster sprite at the current position
    //DrawCircleV(position, 10, RED); // Debugging purposes, comment out when not needed
}

void Monster::SetGoal(TileCoordinates goal) // Set the monster goal tile and call the PathFinding to the monsterGoal
{
	monsterGoal = goal; // set the monsterGoal to the goal passed along as a TileCoordinate
	MonsterPath(); // call MonsterPath function
}

void Monster::SetPath(const std::vector<TileCoordinates>& path)
{
    monsterPath = path;
}

void Monster::SetTileSize(Vector2 tileSize) // Set the size of the tile for the monster PathFinding
{
	monsterTileSize = tileSize; // assign monster tileSize according to the tileSize being passed to the function
}
// Assignment 2 - Part 5
void Monster::MonsterPath() // Set minotaurStart at the current position for it and move towards the MonsterTileSize goal
{
	// PathFinding from current monster position to the goal (Vector2 & Vector2 data type)
	TileCoordinates minotaurStart = TileCoordinates::ScreenToTileCoordinates(position, monsterTileSize);
    monsterPathFinder = PathFinder(&tilemap, minotaurStart, monsterGoal);
    monsterPathFinder.SolvePath();
}
// Assignment 2 - Part 5
void Monster::MonsterUpdate(float deltaTime)
{
    // check monsterPath tiles
    if (!monsterPath.empty())
    {
        // Create vector to go through each tile on the monster path
        Vector2 goalPos = TileCoordinates::TileToScreenCoordinates(monsterPath.front(), monsterTileSize);
        // direction = goalPos - position (result = { v1.x - v2.x, v1.y - v2.y };)
        Vector2 direction = Vector2Subtract(goalPos, position);

        // normalized direction vector
        direction = Vector2Normalize(direction);

        // scaled direction vector (monsterSpeed * deltaTime)
        direction = Vector2Scale(direction, monsterSpeed * deltaTime);

        // position  = position + direction (result = { v1.x + v2.x, v1.y + v2.y };)
        position = Vector2Add(position, direction);

        // if we've reached the goal tile, remove it from the path
        if (Vector2Distance(position, goalPos) < monsterSpeed * deltaTime)
        {
            position = goalPos; // snap the monster to the goal to prevent overshooting
            monsterPath.erase(monsterPath.begin());
        }
    }
}