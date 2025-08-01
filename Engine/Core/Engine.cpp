#include "Engine.h"
#include <iostream>
#include <Windows.h>

#include "Level/Level.h"
#include "Utils/Utils.h"

Engine* Engine::instance = nullptr;

BOOL WINAPI ConsoleMessageProcedure(DWORD CtrlType)
{
	switch (CtrlType)
	{
	case CTRL_CLOSE_EVENT:
		Engine::Get().CleanUp();
		return false;
	}

	return false;
}

Engine::Engine()
{
	instance = this;

	CONSOLE_CURSOR_INFO info;
	info.bVisible = false;
	info.dwSize = 1;

	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
	SetConsoleCtrlHandler(ConsoleMessageProcedure, TRUE);

	LoadEngineSettings();

	srand(static_cast<unsigned int>(time(nullptr)));
}

Engine::~Engine()
{
	CleanUp();
}

void Engine::Run()
{
	LARGE_INTEGER currentTime;
	LARGE_INTEGER previousTime;
	QueryPerformanceCounter(&currentTime);
	previousTime = currentTime;

	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	float targetFrameRate = settings.framerate == 0.0f ? 60.0f : settings.framerate;
	float oneFrameTime = 1.0f / targetFrameRate;

	while (true)
	{
		if (isQuit) break;

		QueryPerformanceCounter(&currentTime);

		float deltaTime = (currentTime.QuadPart - previousTime.QuadPart) / (float)frequency.QuadPart;

		input.ProcessInput();

		if (deltaTime >= oneFrameTime)
		{
			BeginPlay();
			Tick(deltaTime);
			Render();

			previousTime = currentTime;

			input.SavePreviousKeyStates();

			if (mainLevel)
			{
				mainLevel->ProcessAddAndDestroyActors();
			}
		}
	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void Engine::AddLevel(Level* newLevel)
{
	if (mainLevel)
	{
		delete mainLevel;
	}
	
	mainLevel = newLevel;
}

void Engine::CleanUp()
{
	SafeDelete(mainLevel);
}

void Engine::Quit()
{
	isQuit = true;
}

Engine& Engine::Get()
{
	return *instance;
}

int Engine::Width() const
{
	return settings.width;
}

int Engine::Height() const
{
	return settings.height;
}



void Engine::BeginPlay()
{
	if (mainLevel) mainLevel->BeginPlay();
}

void Engine::Tick(float deltaTime)
{
	if (mainLevel) mainLevel->Tick(deltaTime);
}

void Engine::Render()
{
	Utils::SetConsoleTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	if (mainLevel) mainLevel->Render();
}

void Engine::LoadEngineSettings()
{
	FILE* file = nullptr;
	fopen_s(&file, "../Settings/EngineSettings.txt", "rt");
	
	if (file == nullptr)
	{
		std::cout << "Failed to load engine settings." << '\n';
		__debugbreak();
		return;
	}
	
	fseek(file, 0, SEEK_END);
	size_t fileSize = ftell(file);
	rewind(file);

	char* buffer = new char[fileSize + 1];
	memset(buffer, 0, fileSize + 1);

	size_t readSize = fread(buffer, sizeof(char), fileSize, file);

	char* context = nullptr;
	char* token = nullptr;
	token = strtok_s(buffer, "\n", &context);

	while (token != nullptr)
	{
		char header[10] = { };
		sscanf_s(token, "%s", header, 10);

		if (strcmp(header, "framerate") == 0)
		{
			sscanf_s(token, "framerate = %f", &settings.framerate);
		}
		else if (strcmp(header, "width") == 0)
		{
			sscanf_s(token, "width = %d", &settings.width);
		}
		else if (strcmp(header, "height") == 0)
		{
			sscanf_s(token, "height = %d", &settings.height);
		}

		token = strtok_s(nullptr, "\n", &context);
	}

	SafeDeleteArray(buffer);

	fclose(file);
}
