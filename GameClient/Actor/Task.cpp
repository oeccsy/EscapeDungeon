#include "Task.h"

#include "Math/Vector2.h"
#include "Collider/BoxCollider.h"

#include "Utils/Utils.h"
#include <iostream>


const float Task::REQUIRED_TASK_TIME = 5.0f;

Task::Task(const Vector2& position) : Actor("T", Color::White, position), curTaskTime(0.0f)
{
	SetSortingOrder(2);
	collider = new BoxCollider({ 0, 0 }, { 0, 0 }, this);
}

void Task::ProgressTask(float deltaTime)
{
	if (curTaskTime >= REQUIRED_TASK_TIME) return;

	curTaskTime += deltaTime;
	if (curTaskTime >= REQUIRED_TASK_TIME) TaskDone();

	char buffer[20] = { };
	sprintf_s(buffer, sizeof(buffer), "%f", curTaskTime);

	Utils::SetConsolePosition(Vector2(100, 5));
	Utils::SetConsoleTextColor(Color::White);
	std::cout << buffer;
}

void Task::TaskDone()
{
	SafeDeleteArray(image);

	image = new char[2];
	strcpy_s(image, 2, "D");
}