#pragma once

#include <Windows.h>
#include <Xinput.h>

constexpr auto STICK_LEFT = 0x0001;
constexpr auto STICK_RIGHT = 0x0002;

constexpr auto LEFT_THUMB_REST = (INT64)0x1 << 16;
constexpr auto LEFT_THUMB_UP = (INT64)0x2 << 16;
constexpr auto LEFT_THUMB_UPRIGHT = (INT64)0x4 << 16;
constexpr auto LEFT_THUMB_RIGHT = (INT64)0x8 << 16;
constexpr auto LEFT_THUMB_DOWNRIGHT = (INT64)0x10 << 16;
constexpr auto LEFT_THUMB_DOWN = (INT64)0x20 << 16;
constexpr auto LEFT_THUMB_DOWNLEFT = (INT64)0x40 << 16;
constexpr auto LEFT_THUMB_LEFT = (INT64)0x80 << 16;
constexpr auto LEFT_THUMB_UPLEFT = (INT64)0x100 << 16;

constexpr auto RIGHT_THUBM_REST = (INT64)0x1 << 28;
constexpr auto RIGHT_THUMB_UP = (INT64)0x2 << 28;
constexpr auto RIGHT_THUMB_UPRIGHT = (INT64)0x4 << 28;
constexpr auto RIGHT_THUMB_RIGHT = (INT64)0x8 << 28;
constexpr auto RIGHT_THUMB_DOWNRIGHT = (INT64)0x10 << 28;
constexpr auto RIGHT_THUMB_DOWN = (INT64)0x20 << 28;
constexpr auto RIGHT_THUMB_DOWNLEFT = (INT64)0x40 << 28;
constexpr auto RIGHT_THUMB_LEFT = (INT64)0x80 << 28;
constexpr auto RIGHT_THUMB_UPLEFT = (INT64)0x100 << 28;

constexpr auto THUMB_MODE_FOURTHS = 0x0;
constexpr auto THUMB_MODE_EIGHTHS = 0x1;

class XInputController
{
private:
	int _userIndex = 0;
	int _mode = THUMB_MODE_EIGHTHS;

	XINPUT_STATE _state;

	int _leftStickDeadzone = 5000;
	int _rightStickDeadzone = 5000;
private:
	INT64 getStickDirection(int stick);
	bool isStickDead(int stick);
public:
	XInputController(int userIndex);

	void UpdateState();
	XINPUT_STATE GetState();
	INT64 GetLongState();
};

