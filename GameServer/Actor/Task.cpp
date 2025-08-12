#include "Task.h"

#include "Math/Vector2.h"
#include "Collider/BoxCollider.h"

#include "Utils/Utils.h"
#include "Utils/Logs.h"
#include <iostream>


const float Task::REQUIRED_TASK_TIME = 5.0f;
int Task::completedTaskCount = 0;

Task::Task(const Vector2& position) : Actor("T", Color::Blue, position), curTaskTime(0.0f)
{
	SetSortingOrder(2);
	collider = new BoxCollider({ 0, 0 }, { 0, 0 }, this);
}

void Task::ProgressTask(float deltaTime)
{
	if (curTaskTime >= REQUIRED_TASK_TIME) return;

	curTaskTime += deltaTime;
	if (curTaskTime >= REQUIRED_TASK_TIME)
	{
		TaskDone();

		//char buffer[30];
		//sprintf_s(buffer, sizeof(buffer), "Task 완료! : %f", curTaskTime);
		//Utils::SetConsolePosition(Vector2(90, 5));
		//Utils::SetConsoleTextColor(Color::White);
		//std::cout << buffer;
	}
	else
	{
		//char buffer[30];
		//sprintf_s(buffer, sizeof(buffer), "Task 수행중 ... : %f", curTaskTime);
		//Utils::SetConsolePosition(Vector2(90, 5));
		//Utils::SetConsoleTextColor(Color::White);
		//std::cout << buffer;
	}
}

void Task::TaskDone()
{
	SafeDeleteArray(image);

	image = new char[2];
	strcpy_s(image, 2, "D");

	++completedTaskCount;

	Logs::Get().AddLog({ "누군가 Task를 완료했다." });
}