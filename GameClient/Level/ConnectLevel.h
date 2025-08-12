#pragma once

#include "Level/Level.h"
#include "System/UISystem.h"
#include "System/CommandHandler.h"

#include <vector>
#include <string>

class ConnectLevel : public Level
{
	RTTI_DECLARATIONS(ConnectLevel, Level)

public:
	ConnectLevel();
	~ConnectLevel();

	virtual void Tick(float deltaTime) override;
	virtual void Render() override;

	void NewPlayerJoined(int playerCount);
	void GameStart(int index);

private:
	void RecvData();

	void RequestConnect();
	void RequestReady();
	void RequestGameStart();

public:
	static const int MAX_PLAYER_COUNT = 2;

private:
	int playerCount = 0;

	UISystem uiSystem;
	CommandHandler commandHandler;
};