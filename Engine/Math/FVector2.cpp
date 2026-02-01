#include "FVector2.h"
#include <cmath>

FVector2::FVector2(float x, float y) : x(x), y(y) { }

FVector2::~FVector2() { }

FVector2 FVector2::operator+(const FVector2& other) const
{
    return FVector2(x + other.x, y + other.y);
}

FVector2 FVector2::operator-(const FVector2& other) const
{
    return FVector2(x - other.x, y - other.y);
}

FVector2 FVector2::operator*(const int num) const
{
    return FVector2(x * num, y * num);
}

FVector2 FVector2::operator*(const float num) const
{
    return FVector2(x * num, y * num);
}

FVector2 FVector2::operator/(const int num) const
{
    return FVector2(x / num, y / num);
}

FVector2 FVector2::operator/(const float num) const
{
    return FVector2(x / num, y / num);
}

bool FVector2::operator==(const FVector2& other) const
{
    return x == other.x && y == other.y;
}

float FVector2::Dot(const FVector2& other) const
{
    return x * other.x + y * other.y;
}

float FVector2::Length()
{
    return std::sqrt(x * x + y * y);
}

void FVector2::Normalize()
{
    float length = Length();
    if (length <= 1e-6f) return;
    
    x /= length;
    y /= length;
}
