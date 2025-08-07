#include "Timer.h"

Timer::Timer() : targetTime(0.0f), elapsedTime(0.0f), isRunning(false) { }

void Timer::Tick(float deltaTime)
{
    if (isRunning = false) return;
   
    elapsedTime += deltaTime;

    if (targetTime <= elapsedTime)
    {
        Notify();
        Restart();
    }
}

void Timer::SetTimer(float targetTime)
{
    this->targetTime = targetTime;
    isRunning = true;
}

void Timer::Register(Delegate delegate)
{
    onTimeOut.push_back(delegate);
}

void Timer::Notify()
{
    for (auto delegate : onTimeOut)
    {
        delegate.Invoke();
    }
}


void Timer::Restart()
{
    elapsedTime = 0.0f;
}

