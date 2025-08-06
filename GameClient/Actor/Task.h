#pragma once

#include "Actor/Actor.h"

class Task : public Actor
{
	RTTI_DECLARATIONS(Task, Actor)

public:
	Task(const Vector2& position);

	void ProgressTask(float deltaTime);
	
private:
	void TaskDone();

public:
	static const int MAX_TASK_COUNT = 3;
	static const int REQUIRED_TASK_COUNT = 1;
	
	static int completedTaskCount;

private:
	static const float REQUIRED_TASK_TIME;
	float curTaskTime;
};