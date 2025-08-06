#pragma once

#include "Core.h"
#include "Math/Vector2.h"

class Actor;
class Engine_API BoxCollider
{
public:
	BoxCollider(Vector2 center, Vector2 extends);
	BoxCollider(Vector2 center, Vector2 extends, Actor* owner);
	~BoxCollider() = default;

	Vector2 GetMaxBound();
	Vector2 GetMinBound();

	bool Intersects(BoxCollider* other);
	bool Intersects(Vector2 pos);
	
private:
	Vector2 center;
	Vector2 extends;

	Actor* owner;
};