#pragma once

#include "../Noise/PerlinNoise.h"
#include <vector>

struct DungeonNode
{
	int row;
	int col;
	int id;
};

class DungeonExportSystem
{
public:
	DungeonExportSystem();
	~DungeonExportSystem();
	
	void GenerateDungeon(PerlinNoise& perlinNoise);
	void Export();
	void Print();

private:
	void Union(int id1, int id2, std::vector<int>& parents);
	int Find(int id, std::vector<int>& parents);

private:
	int width = 0;
	int height = 0;
	char dungeon[100][100] = { };
};