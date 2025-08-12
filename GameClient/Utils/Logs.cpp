#include "Logs.h"
#include "Utils/Utils.h"

#include <iostream>

Logs* Logs::instance = nullptr;

Logs::Logs() { instance = this; }

Logs::~Logs() { }

void Logs::AddLog(Log log)
{
	logs.push_back(log);
	Render();
}

void Logs::AddLog(char data[100])
{
	Log log;
	memcpy(log.data, data, sizeof(log.data));
	
	logs.push_back(log);
	Render();
}


void Logs::Export()
{
	FILE* file = nullptr;

	char fullPath[32];
	const char* path = "../Assets/";
	sprintf_s(fullPath, sizeof(fullPath), "%sLog.txt", path);

	fopen_s(&file, fullPath, "wt");

	if (file == nullptr)
	{
		AddLog({ "로그 파일 생성 실패" });
		return;
	}

	for (auto& log : logs)
	{
		fwrite(log.data, sizeof(char), sizeof(log.data), file);
		fputc('\n', file);
	}

	fclose(file);

	AddLog({ "로그 파일 생성 성공" });
}

Logs& Logs::Get()
{
	return *instance;
}

void Logs::Render()
{
	Utils::SetConsoleTextColor(Color::White);

	for (int i = 1; i <= 20; ++i)
	{
		if (logs.size() < i) break;

		Log log = logs[logs.size() - i];
		Utils::SetConsolePosition(Vector2(92, 38 - i));
		std::cout << "                                    ";
		Utils::SetConsolePosition(Vector2(92, 38 - i));
		std::cout << log.data;
	}
}
