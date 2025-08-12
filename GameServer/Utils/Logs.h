#pragma once

#include <vector>

struct Log
{
	char data[100];
};

class Logs
{
public:
	Logs();
	~Logs();

	void AddLog(Log log);
	void AddLog(char data[100]);
	void Export();

	static Logs& Get();

private:
	void Render();

private:
	std::vector<Log> logs;

	static Logs* instance;
};