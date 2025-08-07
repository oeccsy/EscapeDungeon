#pragma once

#include "Delegate.h"

#include <vector>

class Timer
{
public:
	Timer();

	void Tick(float deltaTime);

	void SetTimer(float targetTime);
	void Register(Delegate delegate);

private:
	void Notify();
	void Restart();

private:
	float targetTime = 0.0f;
	float elapsedTime = 0.0f;

	bool isRunning = false;

	std::vector<Delegate> onTimeOut;
};