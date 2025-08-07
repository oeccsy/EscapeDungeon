#pragma once

#include "Level/Level.h"
#include "Networking/Client.h"

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
	
	void RecvData();

	void SetPlayerInfo(int playerCount);

	void RequestConnect();
	void RequestPlayersInfo();

private:
	Client client;
	int playerCount = 0;
	std::vector<std::string> logs;
};