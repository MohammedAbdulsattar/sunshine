#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "raylib.h"
#include "TileMapping.h"
#include <unordered_map>
#include <set>
#include <vector>

// unordered_map is a Map data structure. It has pairs of Keys and Values. You can access any Value by its Key. All Keys are unique, Values are not.
// Maps are like Dictionaries, where Words are Keys and Definitions are Values.
// All Words (Keys) are unique in the Dictionary, but you might have multiple words that have the same meaning (Value)

// A class which will use either Dijkstra's algorithm or A* to navigate a Tilemap. We could implement Dijkstra's algorithm as a single function,
// but having our Pathfinding algorithm as a class lets us hold onto the information and it provides us a place to read information about intermediary steps for debugging.
// Being a class also lets us perform steps one at a time over time, so we can debug while reading how it is calculating values in the game.
// This is an example of a stateful Object-Oriented Programming approach to something which would otherwise be a stateless function.

class PathFinder
{
public:
	Tilemap* map = nullptr;

private:
	// Maps with TileCoordinates as Keys and Costs as Values. So each unique adjacent stores a value in the map, the cost of the tile {0,2} in the unvisited set is unvisited [{0,2}]
	// Unvisited set stores nodes that have been discovered but not visited directly.
	std::unordered_map <TileCoordinates, float, std::hash<TileCoordinates>, std::equal_to<TileCoordinates>> unvisited;
	// Visited set stores nodes that have been visited. They will never be revisited.
	std::unordered_map<TileCoordinates, float, std::hash<TileCoordinates>, std::equal_to<TileCoordinates>> visited;
	// Value is the TileCoordinate which comes before Key e.g. cheapestEdgeTo [{0,1}] = {0,0} says to get to 0,1 go through 0,0
	std::unordered_map<TileCoordinates, TileCoordinates, std::hash<TileCoordinates>, std::equal_to<TileCoordinates>> cheapestEdgeTo;

	TileCoordinates startNode;
	TileCoordinates endNode;
	TileCoordinates currentNode;
};

#endif // PATHFINDER_H