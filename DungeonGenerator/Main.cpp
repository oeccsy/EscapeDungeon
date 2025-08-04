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
	std::cout << "������ ������ ���� �Է����ּ���. : ";
	std::cin >> repeat;

	DungeonExportSystem exportSystem;
	NoiseSettings settings;
	settings.gridSize = { 4, 4 };
	settings.resolution = { 80, 40 };

	/*std::cout << "������ ���� ���̸� �Է����ּ���. : ";
	std::cin >> settings.resolution.x;
	std::cout << "������ ���� ���̸� �Է����ּ���. : ";
	std::cin >> settings.resolution.y;*/

	while (repeat--)
	{
		settings.seed = Utils::RandomFloat(0.0f, 10000.0f);

		PerlinNoise perlinNoise(settings);
		perlinNoise.GeneratePerlinNoise();
		perlinNoise.Print();
	
		exportSystem.GenerateDungeon(perlinNoise);
	}

	return 0;
}