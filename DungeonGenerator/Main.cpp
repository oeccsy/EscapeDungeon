#include <iostream>
#include <cmath>
#include <algorithm>

#include "Math/Vector2.h"
#include "Math/FVector2.h"
#include "Math/Mathf.h"

#include "Noise/PerlinNoise.h"

int main()
{
	NoiseSettings settings;
	settings.resolution = { 80, 40 };
	settings.gridSize = { 4, 4 };
	settings.seed = 0.1f;

	PerlinNoise* perlinNoise = new PerlinNoise(settings);
	perlinNoise->GeneratePerlinNoise();
	perlinNoise->Print();

	delete perlinNoise;

	return 0;
}