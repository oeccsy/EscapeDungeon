#pragma once

#include "Core.h"
#include "Input.h"
#include <Windows.h>

struct EngineSettings
{
	int width = 0;
	int height = 0;

	float framerate = 0.0f;
};

class Level;
class Engine_API Engine
{
public:
	Engine();
	virtual ~Engine();

	void Run();

	void AddLevel(Level* newLevel);

	virtual void CleanUp();
	void Quit();

	int Width() const;
	int Height() const;

	static Engine& Get();

private:
	void ProcessInput();
	void BeginPlay();
	void Tick(float deltaTime = 0.0f);
	void Render();

	void LoadEngineSettings();

protected:
	bool isQuit = false;

	Level* mainLevel = nullptr;

	Input input;

	EngineSettings settings;

	static Engine* instance;
};