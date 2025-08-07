#include <iostream>
#include <cmath>
#include <algorithm>

#include "Math/Vector2.h"
#include "Math/FVector2.h"
#include "Math/Mathf.h"
#include "Utils/Utils.h"

#include "Noise/PerlinNoise.h"
#include "System/DungeonExportSystem.h"

int main()
{
	int repeat;
	std::cout << "생성할 던전의 수를 입력해주세요. : ";
	std::cin >> repeat;

	DungeonExportSystem exportSystem;
	NoiseSettings settings;
	settings.gridSize = { 4, 4 };
	settings.resolution = { 80, 40 };

	srand(static_cast<unsigned int>(time(nullptr)));

	while (repeat--)
	{
		settings.seed = Utils::RandomFloat(0.0f, 10000.0f);

		PerlinNoise perlinNoise(settings);
		perlinNoise.GeneratePerlinNoise();
		perlinNoise.Print();
	
		exportSystem.GenerateDungeon(perlinNoise);
		exportSystem.Print();
		exportSystem.Export();
	}

	return 0;
}