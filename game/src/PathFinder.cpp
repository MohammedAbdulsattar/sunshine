#include "PathFinder.h"
#include <limits>

// Lab 5 - Part 3
PathFinder::PathFinder()
{
	
}

PathFinder::PathFinder(Tilemap* levelToNavigate, TileCoordinates startTile, TileCoordinates endTile)
{
	map = levelToNavigate; // set map to current level iteration to navigate
	startNode = startTile; // set the start node to the starting player tile position
	endNode = endTile; // set the end node to the tile position the player wishes to reach
	currentNode = startNode; // set the current node to the start node to ensure pathfinding starts at the player's current position
	Restart(); // restart process for new pathfinding
}

void PathFinder::Restart()
{
	visited.clear(); // Clear all visited Tiles stored in the unordered_map for them
	cheapestEdgeTo.clear(); // clear the cheapestEdgeTo path from startNode to endNode stored in the unordered_map for it

	// Reset costs
	unvisited.clear(); // Clear all unvisited Tiles stored in the unordered_map for them
	// obtain a vector container list of currently traversable tiles from the current map
	for (TileCoordinates position: map->GetAllTraversableTiles()) // go through each TileCoordinate position in the current map
	{
		// the [] operator in std::unordered_map gets or adds values by key. Be sure to read the documentation on it!
		// If you try to access a key which is not present in the container with [], it will make one.
		// (This also means it SHOULD NOT be used to check if a key exists!)
		unvisited[position] = INFINITY; // set each unvisited position cost to Infinity
	}
	unvisited[startNode] = 0; // reset the value of the unvisited startNode to 0 where the pathfinding begins
}

// Lab 5 - Part 5
// Return the adjacent of the node with the lowest cost in the set.
// If the set is empty the lowest cost will be INFINITY and node will be {-1,-1}
std::pair<TileCoordinates,float> PathFinder::GetLowestCostIn(std::unordered_map<TileCoordinates, float> set)
{
	// The default "cheapestPosition" return value is {-1,-1} if the set is empty or all costs are INFINITY
	TileCoordinates cheapestPosition = { -1,-1 }; // This is because the starting position tile is {0,0} where the player is located
	float lowestCost = INFINITY; // set the "lowestCost" to an initial value of INFINITY before updated through iteration
	// nodeValuePair contains "TileCoordinates", as the first element, and "cost", as the second element, alongside the first element
	for (auto nodeValuePair : set) // Loop through each value pair set
	{
		TileCoordinates tile = nodeValuePair.first; // first stored value (tile position) in the value pair elements
		float cost = nodeValuePair.second; // second stored value (cost of the associated tile) in the value pair elements

		if (cost < lowestCost) // if the "cost" is less than "lowestCost" found during current iteration
		{
			cheapestPosition = tile; // set the "cheapestPosition" to current "tile"
			lowestCost = cost; // set the "lowestCost" (current) as the new "cost"
		}
	}
	// return a pair of both the current cheapestPosition and its associated lowestCost together as a set
	return { cheapestPosition, lowestCost }; // Format [TileCoordinate, Cost]
}
 
// Lab 5 - Part 6
bool PathFinder::IsVisited(TileCoordinates pos) const // check if a Tile position has been "visited"
{
	return visited.count(pos); // return the tile position visit as true in the "visited" vector
}

bool PathFinder::IsSolved() const // check if the pathfinding algorithm has reached the "endNode"
{
	return IsVisited(endNode); // return true if the visit position is set to true and exists in the "visited" vector
}

bool PathFinder::IsCompleted() // check if the pathfinding algorithm is finished
{
	// return true if the the "endNode" is set to true as "visited" in the vector or if all "unvisited" nodes have INFINITY cost
	return IsVisited(endNode) || GetLowestCostIn(unvisited).second == INFINITY;
}

// Lab 5 - Part 7
void PathFinder::MoveToVisitedSet(TileCoordinates node)
{
	visited[currentNode] = unvisited[currentNode]; // set the "unvisited" "currentNode" as a "visisted" node adding it to the vector
	unvisited.erase(currentNode); // erase the "currentNode" from the "unvisited" vector
}

void PathFinder::ProcessNextIterationFunctional()
{
	if (IsCompleted()) // if PathFinding is Complete then just return without doing anything
		return;

	currentNode = GetLowestCostIn(unvisited).first;

	// TODO: evaluate costs to reach this Node's neighbors and update
	// Lab 5 - Part 8
	for (auto adjacent : map->GetTraversibleTilesAdjacentTo(currentNode))
	{
		// If we have already visited this, skip it
		if (IsVisited(adjacent))
			continue;

		// Create node with new cost
		float costThisWay = GetTotalCostToReach(currentNode) + map->GetCostForTile(adjacent);

		float oldCost = GetTotalCostToReach(adjacent);
		if (costThisWay < oldCost)
		{
			SetCostToReach(adjacent, costThisWay);
			cheapestEdgeTo[adjacent] = currentNode;
		}

		MoveToVisitedSet(currentNode); // Done exploring this. Remove current node from "Unvisited", place it in "Visited"
	}

	MoveToVisitedSet(currentNode); // Done exploring this. Remove current node from "Unvisited", place it in "Visited"
		
}

bool PathFinder::SolvePath()
{
	Restart(); // restart entire algorithm

	while(!IsCompleted()) // while the algorithm is not complete and hasn't reached the endNode
	{
		ProcessNextIterationFunctional(); // continue algorithm and evaluate the next nodes in the vectors ("visited" & "unvisited")
	}

	return IsSolved(); // return the "endNode" if algorithm succeeds and reaches the end position
}

// Lab 5 - Part 8
float PathFinder::GetTotalCostToReach(TileCoordinates pos)
{
	return unvisited[pos];
}

void PathFinder::SetCostToReach(TileCoordinates pos, float newCost)
{
	unvisited[pos] = newCost;
}