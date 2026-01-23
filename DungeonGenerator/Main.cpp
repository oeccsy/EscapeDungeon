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
	
		exportSystem.GenerateDungeon(perlinNoise);
		exportSystem.AddActors(10, 2);
		exportSystem.Export();
	}

	std::cout << "던전 생성이 완료됐습니다. Enter를 누르면 종료합니다.";
	std::cin.ignore(100, '\n');
	std::cin.get();

	return 0;
}