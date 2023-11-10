#pragma once

#include <Windows.h>
#include <Xinput.h>

constexpr auto STICK_LEFT = 0x0001;
constexpr auto STICK_RIGHT = 0x0002;

class XInputController
{
private:
	int _userIndex = 0;

	XINPUT_STATE _state;

	int _leftStickDeadzone = 2500;
	int _rightStickDeadzone = 2500;
private:
	bool isStickDead(int stick);
public:
	XInputController(int userIndex);

	int GetLeftStickDirection();

	void UpdateState();
	XINPUT_STATE GetState();
	// XINPUT_KEYSTROKE GetKeystroke();
};

