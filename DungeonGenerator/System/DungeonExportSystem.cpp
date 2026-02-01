#include "DungeonExportSystem.h"

#include "Utils/Utils.h"

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>

DungeonExportSystem::DungeonExportSystem() { }

DungeonExportSystem::~DungeonExportSystem() { }

void DungeonExportSystem::GenerateDungeon(PerlinNoise& perlinNoise)
{
	std::cout << "던전 초기화 진행중 ... " << '\n';

	width = perlinNoise.noiseSettings.resolution.x;
	height = perlinNoise.noiseSettings.resolution.y;
	areaCount = 0;

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			dungeon[i][j].row = i;
			dungeon[i][j].col = j;

			if (perlinNoise.noise[i][j] > 1)
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


	std::cout << "영역 구분 중 ..." << '\n';
	int dr[4] = { 0, 0, 1, -1 };
	int dc[4] = { 1, -1, 0, 0 };

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


	std::cout << "최단거리 찾는중 ... " << '\n';

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


	std::cout << "영역 연결중 ..." << '\n';
	
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

	std::cout << "==== 던전 생성 완료! ==== " << '\n';
}

void DungeonExportSystem::AddActors(int taskCount, int exitCount)
{
	std::vector<std::vector<Vector2>> availablePos;
	std::vector<int> availableArea;

	std::cout << "액터 위치 목록 초기화..." << '\n';
	availablePos.resize(areaCount + 1);

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			int areaID = dungeon[i][j].areaID;
			if (areaID > 0) availablePos[areaID].push_back({ i, j });
		}
	}

	for (int i = 1; i <= areaCount; ++i)
	{
		availableArea.push_back(i);
		Utils::ShuffleVector<Vector2>(availablePos[i]);
	}


	{
		std::cout << "Exit 위치 설정중..." << '\n';

		Utils::ShuffleVector<int>(availableArea);
		auto it = availableArea.begin();

		for (int i = 0; i < exitCount; ++i)
		{
			int targetArea = *it;

			Vector2 pos = availablePos[targetArea].back();
			availablePos[targetArea].pop_back();

			dungeon[pos.x][pos.y].value = 'E';

			if (availablePos[targetArea].empty())
			{
				it = availableArea.erase(it);
			}
			else
			{
				it++;
			}

			if (it == availableArea.end()) it = availableArea.begin();
		}
	}


	{
		std::cout << "Task 위치 설정중..." << '\n';

		Utils::ShuffleVector<int>(availableArea);
		auto it = availableArea.begin();

		for (int i = 0; i < taskCount; ++i)
		{
			int targetArea = *it;

			Vector2 pos = availablePos[targetArea].back();
			availablePos[targetArea].pop_back();

			dungeon[pos.x][pos.y].value = 'T';

			if (availablePos[targetArea].empty())
			{
				it = availableArea.erase(it);
			}
			else
			{
				it++;
			}

			if (it == availableArea.end()) it = availableArea.begin();
		}
	}
	

	{
		std::cout << "Player 위치 설정중..." << '\n';

		Utils::ShuffleVector<int>(availableArea);
		auto it = availableArea.begin();
		int playerCount = 1;

		for (int i = 0; i < playerCount; ++i)
		{
			int targetArea = *it;

			Vector2 pos = availablePos[targetArea].back();
			availablePos[targetArea].pop_back();

			dungeon[pos.x][pos.y].value = 'P';

			if (availablePos[targetArea].empty())
			{
				it = availableArea.erase(it);
			}
			else
			{
				it++;
			}

			if (it == availableArea.end()) it = availableArea.begin();
		}

		std::cout << "Monster 위치 설정중..." << '\n';

		int targetArea = *it;

		Vector2 pos = availablePos[targetArea].back();
		availablePos[targetArea].pop_back();

		dungeon[pos.x][pos.y].value = 'M';

		if (availablePos[targetArea].empty())
		{
			it = availableArea.erase(it);
		}
		else
		{
			it++;
		}

		if (it == availableArea.end()) it = availableArea.begin();
	}
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
		std::cout << "파일 생성 실패" << '\n';
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

	std::cout << "파일 생성 성공 : " << fullPath << '\n';
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
