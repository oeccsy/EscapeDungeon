#include "DungeonExportSystem.h"
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>

DungeonExportSystem::DungeonExportSystem() { }

DungeonExportSystem::~DungeonExportSystem() { }

void DungeonExportSystem::GenerateDungeon(PerlinNoise& perlinNoise)
{
	std::cout << "���� �ʱ�ȭ ������ ... " << '\n';

	width = perlinNoise.noiseSettings.resolution.x;
	height = perlinNoise.noiseSettings.resolution.y;

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			dungeon[i][j].row = i;
			dungeon[i][j].col = j;

			if (perlinNoise.noise[i][j] > 0)
			{
				dungeon[i][j].areaID = 0;
				dungeon[i][j].value = '#';
			}
			else
			{
				dungeon[i][j].areaID = -1;
				dungeon[i][j].value = ' ';
			}
		}
	}

	for (int i = 0; i < 100; ++i)
	{
		for (int j = 0; j < 100; ++j)
		{
			DungeonNode dummy;

			areaAdj[i][j].src = dummy;
			areaAdj[i][j].dest = dummy;
			areaAdj[i][j].weight = 10000;
		}
	}

	borderNodes.clear();


	std::cout << "���� ���� �� ..." << '\n';
	int dr[4] = { 0, 0, 1, -1 };
	int dc[4] = { 1, -1, 0, 0 };
	int areaCount = 0;

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			if (dungeon[i][j].areaID != 0) continue;

			++areaCount;

			std::queue<DungeonNode*> queue;
			
			DungeonNode* start = &dungeon[i][j];
			start->areaID = areaCount;
			queue.push(start);

			while (!queue.empty())
			{
				DungeonNode* curNode = queue.front();
				queue.pop();

				bool isEdgeArea = false;

				for (int k = 0; k < 4; k++)
				{
					int nextRow = curNode->row + dr[k];
					int nextCol = curNode->col + dc[k];

					if (nextRow < 0 || nextRow >= height || nextCol < 0 || nextCol >= width) continue;
					if (dungeon[nextRow][nextCol].areaID == -1) isEdgeArea = true;
					if (dungeon[nextRow][nextCol].areaID != 0) continue;

					DungeonNode* nextNode = &dungeon[nextRow][nextCol];
					nextNode->areaID = curNode->areaID;

					queue.push(nextNode);
				}

				if (isEdgeArea) borderNodes.push_back(*curNode);
			}
		}
	}

	if (areaCount >= 100) return;


	std::cout << "�ִܰŸ� ã���� ... " << '\n';

	for (auto& node : borderNodes)
	{
		bool isVisit[100][100] = { false, };

		std::queue<std::pair<DungeonNode*, int>> queue;

		DungeonNode* start = &node;
		isVisit[start->row][start->col] = true;
		queue.push({ start, 0 });

		while (!queue.empty())
		{
			DungeonNode* curNode = queue.front().first;
			int curNodeDist = queue.front().second;
			queue.pop();

			for (int i = 0; i < 4; i++)
			{
				int nextRow = curNode->row + dr[i];
				int nextCol = curNode->col + dc[i];

				if (nextRow < 0 || nextRow >= height || nextCol < 0 || nextCol >= width) continue;
				if (isVisit[nextRow][nextCol]) continue;
				if (dungeon[nextRow][nextCol].areaID == start->areaID) continue;

				isVisit[nextRow][nextCol] = true;

				DungeonNode* nextNode = &dungeon[nextRow][nextCol];
				int nextNodeDist = curNodeDist + 1;
				
				if (nextNode->areaID > 0 && nextNodeDist < areaAdj[start->areaID][nextNode->areaID].weight)
				{
					DungeonEdge* uv = &areaAdj[start->areaID][nextNode->areaID];
					uv->src = *start;
					uv->dest = *nextNode;
					uv->weight = nextNodeDist;

					DungeonEdge* vu = &areaAdj[nextNode->areaID][start->areaID];
					vu->src = *nextNode;
					vu->dest = *start;
					vu->weight = nextNodeDist;
				}
				else
				{
					queue.push({ nextNode, nextNodeDist });
				}
			}
		}
	}


	std::cout << "���� ������ ..." << '\n';
	
	std::priority_queue<std::pair<int, DungeonEdge*>, std::vector<std::pair<int, DungeonEdge*>>, std::greater<std::pair<int, DungeonEdge*>>> pq;

	for (int i = 1; i <= areaCount; i++)
	{
		for (int j = i + 1; j <= areaCount; j++)
		{
			DungeonEdge* edge = &areaAdj[i][j];
			
			if (edge->weight == 0) continue;
			
			pq.push({ edge->weight, edge });
		}
	}


	std::vector<int> parentAreaID;
	int unionCount = 0;

	for (int i = 0; i <= areaCount; ++i)
	{
		parentAreaID.push_back(i);
	}

	while (!pq.empty())
	{
		auto edge = pq.top().second;
		pq.pop();

		int srcAreaID = edge->src.areaID;
		int destAreaID = edge->dest.areaID;

		if (Find(srcAreaID, parentAreaID) == Find(destAreaID, parentAreaID)) continue;

		DungeonNode* prev[100][100] = { nullptr, };

		std::queue<DungeonNode*> queue;
		
		DungeonNode* src = &edge->src;
		prev[src->row][src->col] = src;
		queue.push(src);

		DungeonNode* dest = nullptr;

		while (!queue.empty() && dest == nullptr)
		{
			DungeonNode* curNode = queue.front();
			queue.pop();

			for (int i = 0; i < 4; i++)
			{
				int nextRow = curNode->row + dr[i];
				int nextCol = curNode->col + dc[i];

				if (nextRow < 0 || nextRow >= height || nextCol < 0 || nextCol >= width) continue;
				if (prev[nextRow][nextCol] != nullptr) continue;
				if (dungeon[nextRow][nextCol].areaID == src->areaID) continue;

				prev[nextRow][nextCol] = curNode;

				DungeonNode* nextNode = &dungeon[nextRow][nextCol];
				queue.push(nextNode);

				if (nextNode->areaID == destAreaID) dest = nextNode;
			}
		}

		if (dest == nullptr) continue;
		DungeonNode* path = dest;

		while (path != src)
		{
			dungeon[path->row][path->col].value = '#';
			path = prev[path->row][path->col];
		}

		Union(srcAreaID, destAreaID, parentAreaID);
		unionCount++;

		if (unionCount == areaCount) break;
	}

	std::cout << "==== ���� ���� �Ϸ�! ==== " << '\n';
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
		std::cout << "���� ���� ����" << '\n';
		return;
	}

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			fputc(dungeon[i][j].value, file);
		}
		
		fputc('\n', file);
	}

	fclose(file);

	std::cout << "���� ���� ���� : " << fullPath << '\n';
}

void DungeonExportSystem::Print()
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (dungeon[i][j].areaID >= 0)
			{
				std::cout << dungeon[i][j].areaID;
			}
			else
			{
				std::cout << ' ';
			}

		}
		std::cout << '\n';
	}

	std::cout << '\n';
	std::cout << '\n';

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			std::cout << dungeon[i][j].value;
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
	if (parents[id] != id) parents[id] = Find(parents[id], parents);
	
	return parents[id];
}
