#include "DungeonExportSystem.h"
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>

DungeonExportSystem::DungeonExportSystem() { }

DungeonExportSystem::~DungeonExportSystem() { }

void DungeonExportSystem::GenerateDungeon(PerlinNoise& perlinNoise)
{
	int areaID[100][100];
	std::vector<DungeonNode> edgeArea;
	
	width = perlinNoise.noiseSettings.resolution.x;
	height = perlinNoise.noiseSettings.resolution.y;

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			if (perlinNoise.noise[i][j] > 0)
			{
				areaID[i][j] = 0;
				dungeon[i][j] = '#';
			}
			else
			{
				areaID[i][j] = -1;
				dungeon[i][j] = ' ';
			}
		}
	}


	int dr[4] = { 0, 0, 1, -1 };
	int dc[4] = { 1, -1, 0, 0 };
	int idCount = 0;

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			if (areaID[i][j] != 0) continue;
			
			std::queue<DungeonNode> queue;

			areaID[i][j] = ++idCount;

			DungeonNode start;
			start.row = i;
			start.col = j;
			start.id = areaID[i][j];
			
			queue.push(start);

			while (!queue.empty())
			{
				DungeonNode curNode = queue.front();
				queue.pop();

				bool isEdgeArea = false;

				for (int k = 0; k < 4; k++)
				{
					int nextRow = curNode.row + dr[k];
					int nextCol = curNode.col + dc[k];

					if (nextRow < 0 || nextRow >= height || nextCol < 0 || nextCol >= width) continue;

					if (areaID[nextRow][nextCol] == -1) isEdgeArea = true;
					if (areaID[nextRow][nextCol] != 0) continue;

					areaID[nextRow][nextCol] = areaID[curNode.row][curNode.col];

					DungeonNode nextNode;
					nextNode.row = nextRow;
					nextNode.col = nextCol;
					nextNode.id = areaID[nextRow][nextCol];

					queue.push(nextNode);
				}

				if (isEdgeArea) edgeArea.push_back(curNode);
			}
		}
	}

	std::cout << "edgeArea Size : " << edgeArea.size() << '\n';

	std::vector<int> parentIDs;
	int unionCount = 0;

	for (int i = 0; i <= idCount; ++i)
	{
		parentIDs.push_back(i);
	}
	
	for (auto it = edgeArea.begin(); it != edgeArea.end(); ++it)
	{ 

		DungeonNode* prev[100][100] = { nullptr, };
		std::queue<DungeonNode> queue;

		DungeonNode start = *it;
		prev[start.row][start.col] = new DungeonNode(start);
		queue.push(start);

		DungeonNode end;
		bool findEnd = false;

		while (!queue.empty() && !findEnd)
		{
			DungeonNode curNode = queue.front();
			queue.pop();

			for (int i = 0; i < 4; i++)
			{
				int nextRow = curNode.row + dr[i];
				int nextCol = curNode.col + dc[i];

				if (nextRow < 0 || nextRow >= height || nextCol < 0 || nextCol >= width) continue;
				if (areaID[nextRow][nextCol] == curNode.id) continue;
				if (prev[nextRow][nextCol] != nullptr) continue;
				
				prev[nextRow][nextCol] = new DungeonNode(curNode);
				
				DungeonNode nextNode;
				nextNode.row = nextRow;
				nextNode.col = nextCol;
				nextNode.id = curNode.id;

				queue.push(nextNode);

				int nextID = areaID[nextRow][nextCol];

				if (nextID > 0)
				{
					end = nextNode;
					end.id = areaID[nextRow][nextCol];
					findEnd = true;
				}
			}
		}

		if (!findEnd) continue;
		if (Find(start.id, parentIDs) == Find(end.id, parentIDs)) continue;

		Union(start.id, end.id, parentIDs);
		++unionCount;
		
		DungeonNode* path = &end;
		
		while (true)
		{
			if (path->row == start.row && path->col == start.col) break;
		
			dungeon[path->row][path->col] = '#';
			path = prev[path->row][path->col];
		}

		if (unionCount == idCount) break;
	}


	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (areaID[i][j] >= 0)
			{
				std::cout << areaID[i][j];
			}
			else
			{
				std::cout << ' ';
			}

		}
		std::cout << '\n';
	}

	std::cout << "Area 荐 : " << idCount << '\n';

	Print();
}

void DungeonExportSystem::Export()
{
	FILE* file = nullptr;

	char fullPath[32];
	const char* path = "../Assets/";
	int index = 1;

	while (true)
	{
		sprintf_s(fullPath, sizeof(fullPath), "%sMap_%d.txt", path, index);

		FILE* testFile = nullptr;
		fopen_s(&testFile, fullPath, "r");

		if (testFile == nullptr)
		{
			break;
		}
		else
		{
			fclose(testFile);
			++index;
		}
	}
	
	fopen_s(&file, fullPath, "wt");

	if (file == nullptr)
	{
		std::cout << "颇老 积己 角菩" << '\n';
		return;
	}

	for (int i = 0; i < height; ++i)
	{
		fwrite(dungeon[i], sizeof(char), width, file);
		fputc('\n', file);
	}

	fclose(file);

	std::cout << "颇老 积己 己傍 : " << fullPath << '\n';
}

void DungeonExportSystem::Print()
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			std::cout << dungeon[i][j];
		}
		std::cout << '\n';
	}
}

void DungeonExportSystem::Union(int id1, int id2, std::vector<int>& parents)
{
	int root1 = Find(id1, parents);
	int root2 = Find(id2, parents);

	parents[root2] = root1;
}

int DungeonExportSystem::Find(int id, std::vector<int>& parents)
{
	if (parents[id] == id)
	{
		return id;
	}
	else
	{
		return Find(parents[id], parents);
	}
}
