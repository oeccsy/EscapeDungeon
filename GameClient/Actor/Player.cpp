#include "Player.h"
#include "Input.h"
#include "Interface/IMovable.h"

#include "Math/Vector2.h"
#include "Collider/BoxCollider.h"
#include "Networking/Client.h"
#include "Networking/Command.h"

int Player::escapeCount = 0;
int Player::deadCount = 0;

Player::Player(const Vector2& position, IMovable* movable) : Actor("P", Color::Green, position), movableInterface(movable)
{
	SetSortingOrder(3);

	stamina = 5;
	staminaTimer.SetTimer(0.5f);

	Delegate staminaEvent(this, [](void* instance) { static_cast<Player*>(instance)->AddStamina(); });
	staminaTimer.Register(staminaEvent);

	collider = new BoxCollider({ 0, 0 }, { 1, 1 }, this);
}

void Player::BeginPlay()
{
	super::BeginPlay();
}

void Player::Tick(float deltaTime)
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

void Player::Move()
{
	if (isOwner == false) return;
	if (stamina <= 0) return;

	Client& client = Client::Get();

	if (Input::Get().GetKeyDown(VK_RIGHT))
	{
		Command packet = { };
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
		Command packet = { };
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
		Command packet = { };
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
		Command packet = { };
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

void Player::Escape()
{
	++escapeCount;
	Destroy();
}

void Player::Die()
{
	++deadCount;
	Destroy();
}

int Player::GetStamina()
{
	return stamina;
}

void Player::SetOwner(bool isOwner)
{
	this->isOwner = isOwner;
}

void Player::AddStamina()
{
	if (stamina < MAX_STAMINA)
	{
		++stamina;
	}
}
