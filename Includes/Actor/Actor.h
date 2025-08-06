#pragma once

#include "Core.h"
#include "Math/Vector2.h"
#include "Utils/Color.h"
#include "RTTI.h"

class BoxCollider;
class Engine_API Actor : public RTTI
{
	friend class Level;
	RTTI_DECLARATIONS(Actor, RTTI)

public:
	Actor(const char* image = "", Color color = Color::White, const Vector2& position = Vector2::Zero);
	virtual ~Actor();

	virtual void BeginPlay();
	virtual void Tick(float deltaTime);
	virtual void Render();

	inline bool HasBeganPlay() const { return hasBeganPlay; }

	void SetPosition(const Vector2& newPosition);
	Vector2 GetPosition() const;

	int Width() const;

	void SetSortingOrder(unsigned int sortingOrder);

	void SetOwner(Level* newOwner);
	Level* GetOwner();

	void Destroy();

	void QuitGame();

	BoxCollider* GetBoxCollider();
	bool Intersects(Actor* other);

protected:
	Vector2 position;
	
	char* image = nullptr;

	int width = 0;

	Color color;
	
	bool hasBeganPlay = false;
	
	unsigned int sortingOrder = 0;

	bool isActive = true;
	
	bool isExpired = false;

	Level* owner = nullptr;

	BoxCollider* collider;
};