#include "Monster.h"
#include "Input.h"
#include "Interface/IMovable.h"

#include "Math/Vector2.h"
#include "Collider/BoxCollider.h"
#include "Actor/Player.h"
#include "Networking/Client.h"
#include "Networking/Command.h"
#include "System/CommandHandler.h"

Monster::Monster(const Vector2& position, IMovable* movable) : Actor("M", Color::Red, position), movableInterface(movable)
{
	SetSortingOrder(3);

	stamina = 5;
	staminaTimer.SetTimer(0.25f);

	Delegate staminaEvent(this, [](void* instance) { static_cast<Monster*>(instance)->AddStamina(); });
	staminaTimer.Register(staminaEvent);

	collider = new BoxCollider({ 0, 0 }, { 1, 1 }, this);
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

	if (Input::Get().GetKeyDown(VK_UP))
	{
		Command command;
		command.data[0] = static_cast<char>(CommandType::Up);

		client.writeQueue.push(command);
	}

	if (Input::Get().GetKeyDown(VK_DOWN))
	{
		Command command;
		command.data[0] = static_cast<char>(CommandType::Down);

		client.writeQueue.push(command);
	}
	
	if (Input::Get().GetKeyDown(VK_LEFT))
	{
		Command command;
		command.data[0] = static_cast<char>(CommandType::Left);

		client.writeQueue.push(command);
	}

	if (Input::Get().GetKeyDown(VK_RIGHT))
	{
		Command command;
		command.data[0] = static_cast<char>(CommandType::Right);

		client.writeQueue.push(command);
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

void Monster::SetStamina(int stamina)
{
	this->stamina = stamina;
}

void Monster::SetOwner(bool isOwner)
{
	this->isOwner = isOwner;
}

void Monster::AddStamina()
{
	if (stamina < MAX_STAMINA) ++stamina;
}
