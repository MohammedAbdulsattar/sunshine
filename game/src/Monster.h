#ifndef MONSTER_H
#define MONSTER_H

//#include "rlImGui.h"
//#include "raymath.h"
#include "raylib.h"
//#include "Math.h"
#include <vector>

#include "Character.h"
#include "TileCoordinates.h"
#include "PathFinder.h"

// Assignment 2 - Part 6
class Monster : public Character
{
public:
	Monster(const char* texturePath, Vector2 startPos); // monster filepath constructor

	void Draw() override; // Override base Character draw function for Monster

	void SetGoal(TileCoordinates goal); // set the tile goal for the monster (as a TileCoordinate)

	void SetTileSize(Vector2 tileSize); // Set the tileSize for the monster

	void MonsterPath(); // monster PathFinding

	void MonsterUpdate(float deltaTime); // updating the monsters movement

private:
	TileCoordinates monsterGoal; // monster goal tile
	Vector2 monsterTileSize; // tileSize for the monster Path
	std::vector<TileCoordinates> monsterPath; // monster path vector of (TileCoordinates)
	float monsterSpeed; // speed at which the monster moves

	PathFinder monsterPathFinder; // PathFinding algorithm instance for the monster


};

#endif // MONSTER_H