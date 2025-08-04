#include "PerlinNoise.h"
#include "Math/Mathf.h"

#include <iostream>
#include <cmath>

PerlinNoise::PerlinNoise(Vector2 resolution, Vector2 gridSize) : noiseSettings({resolution, gridSize}) { }

PerlinNoise::PerlinNoise(NoiseSettings settings) : noiseSettings(settings) { }

PerlinNoise::~PerlinNoise() {}

void PerlinNoise::GeneratePerlinNoise()
{
	if (noiseSettings.resolution.x > 100 || noiseSettings.resolution.y > 100) return;

	for (int i = 0; i < noiseSettings.resolution.y; i++)
	{
		for (int j = 0; j < noiseSettings.resolution.x; j++)
		{
			Vector2 coord = { i, j };
			FVector2 uv = { (float)i / noiseSettings.resolution.y * noiseSettings.gridSize.x, (float)j / noiseSettings.resolution.x * noiseSettings.gridSize.y };
			Vector2 gridID = { i * noiseSettings.gridSize.x / noiseSettings.resolution.y, j * noiseSettings.gridSize.y / noiseSettings.resolution.x };
			FVector2 gridUV = { uv.x - std::floor(uv.x), uv.y - std::floor(uv.y) };

			Vector2 topLeft = gridID + Vector2(0, 0);
			Vector2 topRight = gridID + Vector2(0, 1);
			Vector2 bottomLeft = gridID + Vector2(1, 0);
			Vector2 bottomRight = gridID + Vector2(1, 1);

			FVector2 gradTopLeft = GetRandomGradient(topLeft, noiseSettings.seed);
			FVector2 gradTopRight = GetRandomGradient(topRight, noiseSettings.seed);
			FVector2 gradBottomLeft = GetRandomGradient(bottomLeft, noiseSettings.seed);
			FVector2 gradBottomRight = GetRandomGradient(bottomRight, noiseSettings.seed);

			FVector2 offsetToTopLeft = gridUV - Vector2(0.0, 0.0);
			FVector2 offsetToTopRight = gridUV - Vector2(0.0, 1.0);
			FVector2 offsetToBottomLeft = gridUV - Vector2(1.0, 0.0);
			FVector2 offsetToBottomRight = gridUV - Vector2(1.0, 1.0);

			float dotTopLeft = gradTopLeft.Dot(offsetToTopLeft);
			float dotTopRight = gradTopRight.Dot(offsetToTopRight);
			float dotBottomLeft = gradBottomLeft.Dot(offsetToBottomLeft);
			float dotBottomRight = gradBottomRight.Dot(offsetToBottomRight);

			FVector2 smoothGridUV = Mathf::SmoothStep(0.0f, 1.0f, gridUV);

			float left = Mathf::Lerp(dotTopLeft, dotBottomLeft, smoothGridUV.x);
			float right = Mathf::Lerp(dotTopRight, dotBottomRight, smoothGridUV.y);
			float perlin = Mathf::Lerp(left, right, smoothGridUV.y);

			noise[i][j] = static_cast<int>(perlin * 10);
		}
	}
}

void PerlinNoise::Print()
{
	if (noiseSettings.resolution.x > 100 || noiseSettings.resolution.y > 100) return;

	for (int i = 0; i < noiseSettings.resolution.y; i++)
	{
		for (int j = 0; j < noiseSettings.resolution.x; j++)
		{
			if (noise[i][j] > 0)
			{
				std::cout << '#';
			}
			else
			{
				std::cout << ' ';
			}

		}
		std::cout << '\n';
	}
}

FVector2 PerlinNoise::GetRandomGradient(FVector2 position, float seed)
{
	position = position + 0.02f;

	float x = position.Dot(FVector2(123.4f, 234.5f));
	float y = position.Dot(FVector2(234.5f, 345.6f));

	FVector2 gradient = FVector2(sin(x), sin(y)) * 43758.5453f;

	gradient.x = sin(gradient.x + seed);
	gradient.y = sin(gradient.y + seed);

	return gradient;
}
