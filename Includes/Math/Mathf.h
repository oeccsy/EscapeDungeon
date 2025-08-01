#pragma once

#include "FVector2.h"
#include <algorithm>

namespace Mathf
{
	inline float Lerp(float a, float b, float t)
	{
		return a + (b - a) * t;
	}

	inline FVector2 Lerp(const FVector2& a, const FVector2& b, float t)
	{
		return a + (b - a) * t;
	}

	inline float SmoothStep(float start, float end, float x)
	{
		float t = (x - start) / (end - start);
		if (t < 0) t = 0;
		if (1 < t) t = 1;

		return t * t * (3.0f - 2.0f * t);
	}

	inline FVector2 SmoothStep(float start, float end, const FVector2& x)
	{
		return FVector2(SmoothStep(start, end, x.x), SmoothStep(start, end, x.y));
	}
}