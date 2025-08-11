#include "Monster.h"
#include "Input.h"
#include "Interface/IMovable.h"

#include "Math/Vector2.h"
#include "Collider/BoxCollider.h"
#include "Actor/Player.h"
#include "Networking/Client.h"
#include "Networking/Packet.h"

Monster::Monster(const Vector2& position, IMovable* movable) : Actor("M", Color::Red, position), movableInterface(movable)
{
	SetSortingOrder(3);

	stamina = 5;
	staminaTimer.SetTimer(0.25f);

	Delegate staminaEvent(this, [](void* instance) { static_cast<Monster*>(instance)->AddStamina(); });
	staminaTimer.Register(staminaEvent);

	collider = new BoxCollider({ 0, 0 }, { 1, 1 }, this);
}

void Monster::BeginPlay()
{
	super::BeginPlay();
}

void Monster::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		QuitGame();
		return;
	}

	Move();
	staminaTimer.Tick(deltaTime);
}

void Monster::Move()
{
	if (isOwner == false) return;
	if (stamina <= 0) return;

	Client& client = Client::Get();


	if (Input::Get().GetKeyDown(VK_RIGHT))
	{
		Packet packet = { };
		packet.data[0] = 'r';

		client.writeQueue.push(packet);
		//Vector2 targetPos = GetPosition() + Vector2(1, 0);
		//bool movable = movableInterface->Movable(targetPos);

		//if (movable)
		//{
		//	SetPosition(targetPos);
		//	--stamina;
		//}
	}

	if (Input::Get().GetKeyDown(VK_LEFT))
	{
		Packet packet = { };
		packet.data[0] = 'l';

		client.writeQueue.push(packet);
		//Vector2 targetPos = GetPosition() + Vector2(-1, 0);
		//bool movable = movableInterface->Movable(targetPos);

		//if (movable)
		//{
		//	SetPosition(targetPos);
		//	--stamina;
		//}
	}

	if (Input::Get().GetKeyDown(VK_UP))
	{
		Packet packet = { };
		packet.data[0] = 'u';

		client.writeQueue.push(packet);
		//Vector2 targetPos = GetPosition() + Vector2(0, -1);
		//bool movable = movableInterface->Movable(targetPos);

		//if (movable)
		//{
		//	SetPosition(targetPos);
		//	--stamina;
		//}
	}

	if (Input::Get().GetKeyDown(VK_DOWN))
	{
		Packet packet = { };
		packet.data[0] = 'd';

		client.writeQueue.push(packet);
		//Vector2 targetPos = GetPosition() + Vector2(0, 1);
		//bool movable = movableInterface->Movable(targetPos);

		//if (movable)
		//{
		//	SetPosition(targetPos);
		//	--stamina;
		//}
	}
}

void Monster::Kill(Actor* actor)
{
	if (actor == nullptr) return;

	Player* player = actor->As<Player>();

	if (player) player->Die();
}

int Monster::GetStamina()
{
	return stamina;
}

void Monster::SetOwner(bool isOwner)
{
	this->isOwner = isOwner;
}

void Monster::AddStamina()
{
	if (stamina < MAX_STAMINA)
	{
		++stamina;
	}
}
