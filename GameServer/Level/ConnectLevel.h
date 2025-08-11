#pragma once

#include "Level/Level.h"
#include "System/UISystem.h"

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

private:
	void PlayerJoin();

private:
	static const int MAX_PLAYER_COUNT = 4;
	int playerCount = 0;

	UISystem uiSystem;
};