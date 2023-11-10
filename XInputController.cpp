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
int XInputController::getLeftStickDirection()
{
	if (isStickDead(STICK_LEFT)) {
		return 0;
	}

	int eighth = (fmod(atan2(_state.Gamepad.sThumbLX, _state.Gamepad.sThumbLY) * 180 / PI + 382.5, 360) / 45 + 1);
	switch (eighth)
	{
	case 1:
		return LEFT_THUMB_UP;
	case 2:
		return LEFT_THUMB_UPRIGHT;
	case 3:
		return LEFT_THUMB_RIGHT;
	case 4:
		return LEFT_THUMB_DOWNRIGHT;
	case 5:
		return LEFT_THUMB_DOWN;
	case 6:
		return LEFT_THUMB_DOWNLEFT;
	case 7:
		return LEFT_THUMB_LEFT;
	case 8:
		return LEFT_THUMB_UPLEFT;
	}
}

int XInputController::getRightStickDirection() {
	if (isStickDead(STICK_RIGHT)) {
		return 0;
	}

	int eighth = (fmod(atan2(_state.Gamepad.sThumbRX, _state.Gamepad.sThumbRY) * 180 / PI + 382.5, 360) / 45 + 1);
	switch (eighth)
	{
	case 1:
		return RIGHT_THUMB_UP;
	case 2:
		return RIGHT_THUMB_UPRIGHT;
	case 3:
		return RIGHT_THUMB_RIGHT;
	case 4:
		return RIGHT_THUMB_DOWNRIGHT;
	case 5:
		return RIGHT_THUMB_DOWN;
	case 6:
		return RIGHT_THUMB_DOWNLEFT;
	case 7:
		return RIGHT_THUMB_LEFT;
	case 8:
		return RIGHT_THUMB_UPLEFT;
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

long XInputController::GetLongState()
{
	return _state.Gamepad.wButtons + getLeftStickDirection() + getRightStickDirection();
}
