#include "Actor.h"
#include "Utils/Utils.h"
#include "Engine.h"
#include "Level/Level.h"

#include <Windows.h>
#include <iostream>

Actor::Actor(const char* image, Color color, const Vector2& position) : color(color), position(position)
{
	width = (int)strlen(image);

	this->image = new char[width + 1];

	strcpy_s(this->image, width + 1, image);
}

Actor::~Actor()
{
	SafeDeleteArray(image);
}

void Actor::BeginPlay()
{
	hasBeganPlay = true;
}

void Actor::Tick(float deltaTime)
{

}

void Actor::Render()
{
	Utils::SetConsolePosition(position);
	Utils::SetConsoleTextColor(color);

	std::cout << image;
}

void Actor::SetPosition(const Vector2& newPosition)
{
	if (newPosition.x < 0) return;
	if (newPosition.x + width - 1 > Engine::Get().Width()) return;
	if (newPosition.y < 0) return;
	if (newPosition.y - 1 > Engine::Get().Height()) return;

	if (position == newPosition)
	{
		return;
	}

	Vector2 direction = newPosition - position;
	position.x = direction.x >= 0 ? position.x : position.x + width - 1;

	Utils::SetConsolePosition(position);
	
	// TODO : 문자열 길이 고려해서 지울 위치 확인해야함.
	std::cout << ' ';

	position = newPosition;
}

Vector2 Actor::GetPosition() const
{
	return position;
}

int Actor::Width() const
{
	return width;
}

void Actor::SetSortingOrder(unsigned int sortingOrder)
{
	this->sortingOrder = sortingOrder;
}

void Actor::SetOwner(Level* newOwner)
{
	owner = newOwner;
}

Level* Actor::GetOwner()
{
	return owner;
}

bool Actor::TestIntersect(const Actor* const other)
{
	// AABB(Axis Aligned Bounding Box).
	// Note: 현재 액터 구조 상 세로는 크기가 없음(크기가 1).
	//       따라서 가로의 최소/최대 위치만 더 고려하면 됨.

	// 이 액터의 x 좌표 정보.
	int xMin = position.x;
	int xMax = position.x + width - 1;

	// 충돌 비교할 다른 액터의 x 좌표 정보.
	int otherXMin = other->position.x;
	int otherXMax = other->position.x + other->width - 1;

	// 안겹치는 조건 확인.

	// 다른 액터의 왼쪽 좌표가 내 오른쪽 좌표보다 더 오른쪽에 있으면 안겹침.
	if (otherXMin > xMax)
	{
		return false;
	}

	// 다른 액터의 오른쪽 좌표가 내 왼쪽 좌표보다 더 왼쪽에 있으면 안겹침.
	if (otherXMax < xMin)
	{
		return false;
	}

	// y 좌표가 같은지 최종 확인.
	return position.y == other->position.y;
}

void Actor::Destroy()
{
	isExpired = true;

	owner->DestroyActor(this);
}

void Actor::QuitGame()
{
	Engine::Get().Quit();
}
