#pragma once

#include "../Noise/PerlinNoise.h"
#include <vector>

struct DungeonNode
{
	int row;
	int col;

	int areaID = -1;
	char value = ' ';
};

struct DungeonEdge
{
	DungeonNode src;
	DungeonNode dest;
	int weight = 10000;
};

class DungeonExportSystem
{
public:
	DungeonExportSystem();
	~DungeonExportSystem();
	
	void GenerateDungeon(PerlinNoise& perlinNoise);
	void AddActors(int taskCount, int exitCount);
	void Export();
	void Print();

private:
	void Union(int id1, int id2, std::vector<int>& parents);
	int Find(int id, std::vector<int>& parents);

private:
	int width = 0;
	int height = 0;
	int areaCount = 0;

	DungeonNode dungeon[100][100] = { };
	DungeonEdge areaAdj[100][100] = { };
	
	std::vector<DungeonNode> borderNodes;
};