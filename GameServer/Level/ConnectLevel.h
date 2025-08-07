#pragma once

#include "Level/Level.h"
#include "Networking/Server.h"

#include <vector>
#include <string>

class ConnectLevel : public Level
{
	RTTI_DECLARATIONS(ConnectLevel, Level)

public:
	ConnectLevel();
	~ConnectLevel();

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Render() override;

	void InitUI();

	void PlayerJoin();
	void PlayerLeave();

private:
	static const int MAX_PLAYER_COUNT = 4;
	
	int playerCount = 0;
	std::vector<std::string> logs;
};