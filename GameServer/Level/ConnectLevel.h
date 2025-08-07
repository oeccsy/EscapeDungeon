#pragma once

#include "Level/Level.h"
#include "Networking/Server.h"

class ConnectLevel : public Level
{
	RTTI_DECLARATIONS(ConnectLevel, Level)

public:
	ConnectLevel();
	~ConnectLevel();

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Render() override;

	void SendPlayersData();

private:
	Server server;
};