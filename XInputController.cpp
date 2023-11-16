#include <cmath>

#include "XInputController.h"

#define PI 3.14159265

// Checks if stick is outside its dead zone
bool XInputController::isStickDead(int stick)
{
	int x, y, dz;

	if (stick == STICK_LEFT) {
		x = _state.Gamepad.sThumbLX;
		y = _state.Gamepad.sThumbLY;

		dz = _leftStickDeadzone;
	}
	else {
		x = _state.Gamepad.sThumbRX;
		y = _state.Gamepad.sThumbRY;

		dz = _rightStickDeadzone;
	}

	float distance = sqrt(x * x + y * y);

	if (distance > dz) {
		return false;
	}

	return true;
}

XInputController::XInputController(int userIndex)
{
	_userIndex = userIndex;
	_state = XINPUT_STATE{};
}

// Calculates in which eighth the left stick currently is
// 1st is on the top starting -22.5 degrees from y axis
INT64 XInputController::getStickDirection(int stick)
{
	if (isStickDead(stick)) {
		return stick == STICK_LEFT ? LEFT_THUMB_REST : RIGHT_THUBM_REST;
	}

	short x, y;
	if (stick == STICK_LEFT) {
		x = _state.Gamepad.sThumbLX;
		y = _state.Gamepad.sThumbLY;
	}
	else {
		x = _state.Gamepad.sThumbRX;
		y = _state.Gamepad.sThumbRY;
	}

	int res{};
	switch (_mode) {
	case THUMB_MODE_FOURTHS:
		res = fmod(atan2(x, y) * 180 / PI + 405, 360) / 90 + 1;
		res = 2 * res - 1;
		break;
	case THUMB_MODE_EIGHTHS:
		res = fmod(atan2(x, y) * 180 / PI + 382.5, 360) / 45 + 1;
		break;
	}

	INT64 direction{};
	switch (res)
	{
	case 1:
		direction = LEFT_THUMB_UP;
		break;
	case 2:
		direction = LEFT_THUMB_UPRIGHT;
		break;
	case 3:
		direction = LEFT_THUMB_RIGHT;
		break;
	case 4:
		direction = LEFT_THUMB_DOWNRIGHT;
		break;
	case 5:
		direction = LEFT_THUMB_DOWN;
		break;
	case 6:
		direction = LEFT_THUMB_DOWNLEFT;
		break;
	case 7:
		direction = LEFT_THUMB_LEFT;
		break;
	case 8:
		direction = LEFT_THUMB_UPLEFT;
	}

	if (stick == STICK_RIGHT) {
		return direction << 12;
	}
	else {
		return direction;
	}
}

void XInputController::UpdateState()
{
	ZeroMemory(&_state, sizeof(XINPUT_STATE));
	XInputGetState(_userIndex, &_state);
}

XINPUT_STATE XInputController::GetState()
{
	return _state;
}

INT64 XInputController::GetLongState()
{
	return _state.Gamepad.wButtons + getStickDirection(STICK_LEFT) + getStickDirection(STICK_RIGHT);
}
