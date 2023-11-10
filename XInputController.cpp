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
}

// Calculates in which eighth the left stick currently is
// 1st is on the top starting -22.5 degrees from y axis
int XInputController::GetLeftStickDirection()
{
	if (isStickDead(STICK_LEFT)) {
		return 0;
	}

	int eighth = (fmod(atan2(_state.Gamepad.sThumbLX, _state.Gamepad.sThumbLY) * 180 / PI + 382.5, 360) / 45 + 1);
	switch (eighth)
	{
	case 1:
		return VK_PAD_LTHUMB_UP;
	case 2:
		return VK_PAD_LTHUMB_UPRIGHT;
	case 3:
		return VK_PAD_LTHUMB_RIGHT;
	case 4:
		return VK_PAD_LTHUMB_DOWNRIGHT;
	case 5:
		return VK_PAD_LTHUMB_DOWN;
	case 6:
		return VK_PAD_LTHUMB_DOWNLEFT;
	case 7:
		return VK_PAD_LTHUMB_LEFT;
	case 8:
		return VK_PAD_LTHUMB_UPLEFT;
	default:
		return 0;
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

//XINPUT_KEYSTROKE XInputController::GetKeystroke()
//{
//	XINPUT_KEYSTROKE a;
//	int res = XInputGetKeystroke(_userIndex, 0, &a);
//	if (res == ERROR_SUCCESS) {
//		return a;
//	}
//	else {
//		return XINPUT_KEYSTROKE();
//	}
//}
