#include "Vector2.h"
#include <iostream>

Vector2 Vector2::Zero = Vector2(0, 0);
Vector2 Vector2::One = Vector2(1, 1);
Vector2 Vector2::Right = Vector2(0, 1);
Vector2 Vector2::Up = Vector2(1, 0);
        
Vector2::Vector2(int x, int y) : x(x), y(y) { }

Vector2::~Vector2() { }

Vector2 Vector2::operator+(const Vector2& other) const
{
    return Vector2(x + other.x, y + other.y);
}

Vector2 Vector2::operator-(const Vector2& other) const
{
    return Vector2(x - other.x, y - other.y);
}

bool Vector2::operator==(const Vector2& other) const
{
    return x == other.x && y == other.y;
}

bool Vector2::operator!=(const Vector2& other) const
{
    return x != other.x || y != other.y;
}

Vector2::operator COORD()
{
    COORD coord;
    coord.X = (short)x;
    coord.Y = (short)y;

    return coord;
}

Vector2::operator COORD() const
{
    COORD coord;
    coord.X = (short)x;
    coord.Y = (short)y;

    return coord;
}

Vector2::operator FVector2()
{
    FVector2 vec;
    vec.x = (float)x;
    vec.y = (float)y;

    return vec;
}

Vector2::operator FVector2() const
{
    FVector2 vec;
    vec.x = (float)x;
    vec.y = (float)y;

    return vec;
}