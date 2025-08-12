#include "Player.h"
#include "Input.h"
#include "Interface/IMovable.h"

#include "Math/Vector2.h"
#include "Collider/BoxCollider.h"
#include "Networking/Client.h"
#include "Networking/Command.h"
#include "System/CommandHandler.h"

int Player::playerCount = 0;
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

	if (Input::Get().GetKeyDown(VK_UP))
	{
		Command command;
		command.data[0] = static_cast<char>(CommandType::Up);;

		client.writeQueue.push(command);
	}

	if (Input::Get().GetKeyDown(VK_DOWN))
	{
		Command command;
		command.data[0] = static_cast<char>(CommandType::Down);;

		client.writeQueue.push(command);
	}

	if (Input::Get().GetKeyDown(VK_RIGHT))
	{
		Command command;
		command.data[0] = static_cast<char>(CommandType::Right);

		client.writeQueue.push(command);
	}

	if (Input::Get().GetKeyDown(VK_LEFT))
	{
		Command command;
		command.data[0] = static_cast<char>(CommandType::Left);

		client.writeQueue.push(command);
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

void Player::SetStamina(int stamina)
{
	this->stamina = stamina;
}

void Player::SetOwner(bool isOwner)
{
	this->isOwner = isOwner;
}

void Player::AddStamina()
{
	if (stamina < MAX_STAMINA) ++stamina;
}
