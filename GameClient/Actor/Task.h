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

private:
	static const float REQUIRED_TASK_TIME;
	float curTaskTime;
};