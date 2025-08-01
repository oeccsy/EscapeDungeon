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
	
	// TODO : ���ڿ� ���� ����ؼ� ���� ��ġ Ȯ���ؾ���.
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
	// Note: ���� ���� ���� �� ���δ� ũ�Ⱑ ����(ũ�Ⱑ 1).
	//       ���� ������ �ּ�/�ִ� ��ġ�� �� ����ϸ� ��.

	// �� ������ x ��ǥ ����.
	int xMin = position.x;
	int xMax = position.x + width - 1;

	// �浹 ���� �ٸ� ������ x ��ǥ ����.
	int otherXMin = other->position.x;
	int otherXMax = other->position.x + other->width - 1;

	// �Ȱ�ġ�� ���� Ȯ��.

	// �ٸ� ������ ���� ��ǥ�� �� ������ ��ǥ���� �� �����ʿ� ������ �Ȱ�ħ.
	if (otherXMin > xMax)
	{
		return false;
	}

	// �ٸ� ������ ������ ��ǥ�� �� ���� ��ǥ���� �� ���ʿ� ������ �Ȱ�ħ.
	if (otherXMax < xMin)
	{
		return false;
	}

	// y ��ǥ�� ������ ���� Ȯ��.
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
