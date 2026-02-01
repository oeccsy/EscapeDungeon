#pragma once

#include "Core.h"
#include <Windows.h>

class Engine_API FVector2
{
public:
	FVector2(float x = 0.0f, float y = 0.0f);
	~FVector2();

	FVector2 operator+(const FVector2& other) const;
	FVector2 operator-(const FVector2& other) const;
	FVector2 operator*(const int num) const;
	FVector2 operator*(const float num) const;
	FVector2 operator/(const int num) const;
	FVector2 operator/(const float num) const;

	bool operator==(const FVector2& other) const;

	float Dot(const FVector2& other) const;
	float Length();
	void Normalize();

public:
	float x = 0.0f;
	float y = 0.0f;
};