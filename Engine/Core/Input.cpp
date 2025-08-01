#include "Input.h"
#include <Windows.h>

Input* Input::instance = nullptr;

Input::Input()
{
	instance = this;
}

void Input::ProcessInput()
{
	for (int i = 0; i < 255; ++i)
	{
		keyStates[i].isKeyDown = GetAsyncKeyState(i) & 0x8000;
	}
}

void Input::SavePreviousKeyStates()
{
	for (int i = 0; i < 255; ++i)
	{
		keyStates[i].previouseKeyDown = keyStates[i].isKeyDown;
	}
}

bool Input::GetKey(int keyCode)
{
	return keyStates[keyCode].isKeyDown;
}

bool Input::GetKeyDown(int keyCode)
{
	return !keyStates[keyCode].previouseKeyDown && keyStates[keyCode].isKeyDown;
}

bool Input::GetKeyUp(int keyCode)
{
	return keyStates[keyCode].previouseKeyDown && !keyStates[keyCode].isKeyDown;
}

Input& Input::Get()
{
	return *instance;
}