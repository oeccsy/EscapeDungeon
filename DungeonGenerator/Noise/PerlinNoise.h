#pragma once

#include "Math/Vector2.h"
#include "Math/FVector2.h"

struct NoiseSettings
{
	Vector2 resolution = { 80, 40 };
	Vector2 gridSize = { 4, 4 };
	float seed = 0.0f;
};

class PerlinNoise
{
public:
	PerlinNoise(Vector2 resolution, Vector2 gridSize);
	PerlinNoise(NoiseSettings settings);
	~PerlinNoise();

	void GeneratePerlinNoise();
	void Print();

private:
	FVector2 GetRandomGradient(FVector2 position, float seed);

public:
	NoiseSettings noiseSettings;
	int noise[100][100] = { };
};