#pragma once

#include <Windows.h>
#include <Xinput.h>

constexpr auto STICK_LEFT = 0x0001;
constexpr auto STICK_RIGHT = 0x0002;

#define LEFT_THUMB_UP 0x00010000
#define LEFT_THUMB_UPRIGHT 0x00020000
#define LEFT_THUMB_RIGHT 0x00040000
#define LEFT_THUMB_DOWNRIGHT 0x00080000
#define LEFT_THUMB_DOWN 0x00100000
#define LEFT_THUMB_DOWNLEFT 0x00200000
#define LEFT_THUMB_LEFT 0x00400000
#define LEFT_THUMB_UPLEFT 0x00800000

#define RIGHT_THUMB_UP 0x01000000
#define RIGHT_THUMB_UPRIGHT 0x02000000
#define RIGHT_THUMB_RIGHT 0x04000000
#define RIGHT_THUMB_DOWNRIGHT 0x08000000
#define RIGHT_THUMB_DOWN 0x10000000
#define RIGHT_THUMB_DOWNLEFT 0x20000000
#define RIGHT_THUMB_LEFT 0x40000000
#define RIGHT_THUMB_UPLEFT 0x80000000

class XInputController
{
private:
	int _userIndex = 0;

	XINPUT_STATE _state;

	int _leftStickDeadzone = 2500;
	int _rightStickDeadzone = 2500;
private:
	int getLeftStickDirection();
	int getRightStickDirection();
	bool isStickDead(int stick);
public:
	XInputController(int userIndex);

	void UpdateState();
	XINPUT_STATE GetState();
	long GetLongState();
	// XINPUT_KEYSTROKE GetKeystroke();
};

