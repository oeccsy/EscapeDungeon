#pragma once

#include "Level/Level.h"
#include "Gimmick/UISystem.h"

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
	void RecvData();

	void RequestConnect();
	void RequestStart();

private:
	int playerCount = 0;

	UISystem uiSystem;
};