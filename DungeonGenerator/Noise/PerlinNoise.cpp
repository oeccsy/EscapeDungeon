#include "PerlinNoise.h"
#include "Math/Mathf.h"

#include <iostream>
#include <cmath>
#include <algorithm>

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
			FVector2 uv;
			uv.x = static_cast<float>(j) / noiseSettings.resolution.x * noiseSettings.gridSize.x;
			uv.y = static_cast<float>(i) / noiseSettings.resolution.y * noiseSettings.gridSize.y;

			Vector2 gridID;
			gridID.x = static_cast<int>(floor(uv.x));
			gridID.y = static_cast<int>(floor(uv.y));

			FVector2 gridUV;
			gridUV.x = uv.x - std::floor(uv.x);
			gridUV.y = uv.y - std::floor(uv.y);

			Vector2 topLeft = gridID + Vector2(0, 0);
			Vector2 topRight = gridID + Vector2(1, 0);
			Vector2 bottomLeft = gridID + Vector2(0, 1);
			Vector2 bottomRight = gridID + Vector2(1, 1);

			FVector2 gradTopLeft = GetRandomGradient(topLeft, noiseSettings.seed);
			FVector2 gradTopRight = GetRandomGradient(topRight, noiseSettings.seed);
			FVector2 gradBottomLeft = GetRandomGradient(bottomLeft, noiseSettings.seed);
			FVector2 gradBottomRight = GetRandomGradient(bottomRight, noiseSettings.seed);

			FVector2 offsetToTopLeft = gridUV - Vector2(0, 0);
			FVector2 offsetToTopRight = gridUV - Vector2(1, 0);
			FVector2 offsetToBottomLeft = gridUV - Vector2(0, 1);
			FVector2 offsetToBottomRight = gridUV - Vector2(1, 1);

			float dotTopLeft = gradTopLeft.Dot(offsetToTopLeft);
			float dotTopRight = gradTopRight.Dot(offsetToTopRight);
			float dotBottomLeft = gradBottomLeft.Dot(offsetToBottomLeft);
			float dotBottomRight = gradBottomRight.Dot(offsetToBottomRight);

			FVector2 smoothGridUV = Mathf::SmoothStep(0.0f, 1.0f, gridUV);

			float interpolateTop = Mathf::Lerp(dotTopLeft, dotTopRight, smoothGridUV.x);
			float interpolateBottom = Mathf::Lerp(dotBottomLeft, dotBottomRight, smoothGridUV.x);
			float perlin = Mathf::Lerp(interpolateTop, interpolateBottom, smoothGridUV.y);

			perlin = perlin * 20.0f;
			perlin = min(9.9f, max(-9.9f, perlin));
			noise[i][j] = static_cast<int>(perlin); 
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
				std::cout << static_cast<int>(noise[i][j]);
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
	FVector2 adjustedPosition = position + 0.02f;

	float x = adjustedPosition.Dot(FVector2(123.4f, 234.5f));
	float y = adjustedPosition.Dot(FVector2(234.5f, 345.6f));

	FVector2 gradient = FVector2(sin(x), sin(y)) * 43758.5453f;

	gradient.x = sin(gradient.x + seed);
	gradient.y = sin(gradient.y + seed);
	gradient.Normalize();

	return gradient;
}
