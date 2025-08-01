#include <iostream>
#include <cmath>
#include <algorithm>
#include "Math/Vector2.h"
#include "Math/FVector2.h"
#include "Math/Mathf.h"

struct NoiseSettings
{
	Vector2 resolution;
	Vector2 gridSize;
};

struct GridNode
{
	Vector2 coord;
	FVector2 uv;

	Vector2 gridID;
	FVector2 gridUV;
	FVector2 smoothGridUV;
	
	Vector2 topLeft;
	Vector2 topRight;
	Vector2 bottomLeft;
	Vector2 bottomRight;

	FVector2 gradTopLeft;
	FVector2 gradTopRight;
	FVector2 gradBottomLeft;
	FVector2 gradBottomRight;

	float distGradTopLeft;
	float distGradTopRight;
	float distGradBottomLeft;
	float distGradBottomRight;

	int value;
};

NoiseSettings noiseSettings;

GridNode grid[100][100];
int noise[100][100];

void InitGrid()
{
	noiseSettings.resolution = { 50, 50 };
	noiseSettings.gridSize = { 4, 4 };

	for (int i = 0; i < noiseSettings.resolution.y; i++)
	{
		for(int j=0; j< noiseSettings.resolution.x; j++)
		{
			GridNode* curNode = &grid[i][j];
			
			curNode->coord = { i, j };
			curNode->uv = { (float)i / noiseSettings.resolution.x * noiseSettings.gridSize.x, (float)j / noiseSettings.resolution.y * noiseSettings.gridSize.y };

			curNode->gridID = { i * noiseSettings.gridSize.x / noiseSettings.resolution.x, j * noiseSettings.gridSize.y / noiseSettings.resolution.y };
			curNode->gridUV = { curNode->uv.x - std::floor(curNode->uv.x), curNode->uv.y - std::floor(curNode->uv.y)};
			
			curNode->topLeft = curNode->gridID + Vector2(0, 0);
			curNode->topRight = curNode->gridID + Vector2(0, 1);
			curNode->bottomLeft = curNode->gridID + Vector2(1, 0);
			curNode->bottomRight = curNode->gridID + Vector2(1, 1);
		}
	}
}

FVector2 RandomGradient(FVector2 position, float seed)
{
	position = position + 0.02f;

	float x = position.Dot(FVector2(123.4f, 234.5f));
	float y = position.Dot(FVector2(234.5f, 345.6f));

	FVector2 gradient = FVector2(sin(x), sin(y)) * 43758.5453f;

	gradient.x = sin(gradient.x + seed);
	gradient.y = sin(gradient.y + seed);

	return gradient;
}

float OrientedBoxSDF(const FVector2& position, const FVector2& a, const FVector2& b, float thickness)
{
	float len = (b - a).Length();
	FVector2 dir = (b - a) / len;
	FVector2 center = (a + b) * 0.5f;

	FVector2 relativePos = position - center;
	FVector2 rotatedPos = FVector2
		(
			FVector2(dir.x, -dir.y).Dot(relativePos),
			FVector2(dir.y, dir.x).Dot(relativePos)
		);

	FVector2 absPos = FVector2(std::abs(rotatedPos.x), std::abs(rotatedPos.y));
	FVector2 boxMaxBound = FVector2(len / 2, thickness / 2);
	
	FVector2 offset = absPos - boxMaxBound;
	
	float outsideDist = FVector2(max(offset.x, 0.0f), max(offset.y, 0.0f)).Length();
	float insideDist = min(max(offset.x, offset.y), 0.0f);

	return outsideDist + insideDist;
}

void CalcDistToCorner()
{
	for (int i = 0; i < noiseSettings.resolution.y; i++)
	{
		for (int j = 0; j < noiseSettings.resolution.x; j++)
		{
			GridNode* curNode = &grid[i][j];

			curNode->gradTopLeft = RandomGradient(static_cast<FVector2>(curNode->topLeft), 1.0f);
			curNode->gradTopRight= RandomGradient(static_cast<FVector2>(curNode->topRight), 1.0f);
			curNode->gradBottomLeft = RandomGradient(static_cast<FVector2>(curNode->bottomLeft), 1.0f);
			curNode->gradBottomRight = RandomGradient(static_cast<FVector2>(curNode->bottomRight), 1.0f);
			
			// curNode->distGradTopLeft = OrientedBoxSDF(curNode->uv, curNode->topLeft, static_cast<FVector2>(curNode->topLeft) + curNode->gradTopLeft / 2, 0.05f);
			// curNode->distGradTopRight = OrientedBoxSDF(curNode->uv, curNode->topRight, static_cast<FVector2>(curNode->topRight) + curNode->gradTopRight / 2, 0.05f);
			// curNode->distGradBottomLeft = OrientedBoxSDF(curNode->uv, curNode->bottomLeft, static_cast<FVector2>(curNode->bottomLeft) + curNode->gradBottomLeft / 2, 0.05f);
			// curNode->distGradBottomRight = OrientedBoxSDF(curNode->uv, curNode->bottomRight, static_cast<FVector2>(curNode->bottomRight) + curNode->gradBottomRight / 2, 0.05f);
			// 
			// if (curNode->distGradTopLeft < 0 || curNode->distGradTopRight < 0 || curNode->distGradBottomLeft < 0 || curNode->distGradBottomRight < 0) curNode->value = 1;

			FVector2 distFromElemToTopLeft = curNode->gridUV - Vector2(0.0, 0.0);
			FVector2 distFromElemToTopRight = curNode->gridUV - Vector2(0.0, 1.0);
			FVector2 distFromElemToBottomLeft = curNode->gridUV - Vector2(1.0, 0.0);
			FVector2 distFromElemToBottomRight = curNode->gridUV - Vector2(1.0, 1.0);

			float dotTopLeft = curNode->gradTopLeft.Dot(distFromElemToTopLeft);
			float dotTopRight = curNode->gradTopRight.Dot(distFromElemToTopRight);
			float dotBottomLeft = curNode->gradBottomLeft.Dot(distFromElemToBottomLeft);
			float dotBottomRight = curNode->gradBottomRight.Dot(distFromElemToBottomRight);

			curNode->smoothGridUV = Mathf::SmoothStep(0.0f, 1.0f, curNode->gridUV);
			
			float bottomLerp = Mathf::Lerp(dotBottomLeft, dotBottomRight, curNode->smoothGridUV.x);
			float topLerp = Mathf::Lerp(dotTopLeft, dotTopRight, curNode->smoothGridUV.x);
			float perlin = Mathf::Lerp(bottomLerp, topLerp, curNode->smoothGridUV.y);

			int result = (int)(perlin * 10);
			if (result < 0) result = 0;
			if (result > 9) result = 9;

			curNode->value = result;
		}
	}
}

void print()
{
	for (int i = 0; i < noiseSettings.resolution.y; i++)
	{
		for (int j = 0; j < noiseSettings.resolution.x; j++)
		{
			std::cout << grid[i][j].value;
		}
		
		std::cout << '\n';
	}
}

void print(int row, int col)
{
	std::cout << "coord : " << grid[row][col].coord.x << ", " << grid[row][col].coord.y << '\n';
	std::cout << "uv : " << grid[row][col].uv.x << ", " << grid[row][col].uv.y << '\n';
	std::cout << "gridID : " << grid[row][col].gridID.x << ", " << grid[row][col].gridID.y << '\n';
	std::cout << "gridUV : " << grid[row][col].gridUV.x << ", " << grid[row][col].gridUV.y << '\n';
	
	std::cout << '\n';
	std::cout << '\n';

	std::cout << "topLeft : " << grid[row][col].topLeft.x << ", " << grid[row][col].topLeft.y << '\n';
	std::cout << "topRight : " << grid[row][col].topRight.x << ", " << grid[row][col].topRight.y << '\n';
	std::cout << "bottomLeft : " << grid[row][col].bottomLeft.x << ", " << grid[row][col].bottomLeft.y << '\n';
	std::cout << "bottomRight : " << grid[row][col].bottomRight.x << ", " << grid[row][col].bottomRight.y << '\n';


	std::cout << '\n';
	std::cout << '\n';

	std::cout << "gradTopLeft : " << grid[row][col].gradTopLeft.x << ", " << grid[row][col].gradTopLeft.y << '\n';
	std::cout << "gradTopRight : " << grid[row][col].gradTopRight.x << ", " << grid[row][col].gradTopRight.y << '\n';
	std::cout << "gradBottomLeft : " << grid[row][col].gradBottomLeft.x << ", " << grid[row][col].gradBottomLeft.y << '\n';
	std::cout << "gradBottomRight : " << grid[row][col].gradBottomRight.x << ", " << grid[row][col].gradBottomRight.y << '\n';

	std::cout << '\n';
	std::cout << '\n';

	std::cout << "distGradTopLeft : " << grid[row][col].distGradTopLeft << '\n';
	std::cout << "distGradTopRight : " << grid[row][col].distGradTopRight << '\n';
	std::cout << "distGradBottomLeft : " << grid[row][col].distGradBottomLeft << '\n';
	std::cout << "distGradBottomRight : " << grid[row][col].distGradBottomRight << '\n';

	std::cout << '\n';
	std::cout << '\n';
}

int main()
{
	InitGrid();
	CalcDistToCorner();

	print(99, 99);
	print();

	return 0;
}