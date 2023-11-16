#include "KeyboardTranslator.h"

KeyboardTranslator::KeyboardTranslator()
{
	_bindings = {
		{XINPUT_GAMEPAD_DPAD_DOWN, 0xe05b}, // left win
		{XINPUT_GAMEPAD_LEFT_SHOULDER, 0x2a}, // left shift
		{LEFT_THUMB_REST | XINPUT_GAMEPAD_A, 0x1c }, // enter
		{LEFT_THUMB_REST | XINPUT_GAMEPAD_X, 0x0e}, // backspace
		{LEFT_THUMB_REST | XINPUT_GAMEPAD_Y, 0x39}, // space

		{LEFT_THUMB_UP | XINPUT_GAMEPAD_A,		0x10},
		{LEFT_THUMB_UP | XINPUT_GAMEPAD_X,		0x11},
		{LEFT_THUMB_UP | XINPUT_GAMEPAD_Y,		0x12},
		{LEFT_THUMB_UP | XINPUT_GAMEPAD_B,		0x13},

		{LEFT_THUMB_UPRIGHT | XINPUT_GAMEPAD_A,		0x14},
		{LEFT_THUMB_UPRIGHT | XINPUT_GAMEPAD_X,		0x15},
		{LEFT_THUMB_UPRIGHT | XINPUT_GAMEPAD_Y,		0x16},
		{LEFT_THUMB_UPRIGHT | XINPUT_GAMEPAD_B,		0x17},

		{LEFT_THUMB_RIGHT | XINPUT_GAMEPAD_A,		0x18},
		{LEFT_THUMB_RIGHT | XINPUT_GAMEPAD_X,		0x19},
		{LEFT_THUMB_RIGHT | XINPUT_GAMEPAD_Y,		0x1a},
		{LEFT_THUMB_RIGHT | XINPUT_GAMEPAD_B,		0x1b},

		{LEFT_THUMB_DOWNRIGHT | XINPUT_GAMEPAD_A,		0x1e},
		{LEFT_THUMB_DOWNRIGHT | XINPUT_GAMEPAD_X,		0x1f},
		{LEFT_THUMB_DOWNRIGHT | XINPUT_GAMEPAD_Y,		0x20},
		{LEFT_THUMB_DOWNRIGHT | XINPUT_GAMEPAD_B,		0x21},

		{LEFT_THUMB_DOWN | XINPUT_GAMEPAD_A,		0x22},
		{LEFT_THUMB_DOWN | XINPUT_GAMEPAD_X,		0x23},
		{LEFT_THUMB_DOWN | XINPUT_GAMEPAD_Y,		0x24},
		{LEFT_THUMB_DOWN | XINPUT_GAMEPAD_B,		0x25},

		{LEFT_THUMB_DOWNLEFT | XINPUT_GAMEPAD_A,		0x26},
		{LEFT_THUMB_DOWNLEFT | XINPUT_GAMEPAD_X,		0x27},
		{LEFT_THUMB_DOWNLEFT | XINPUT_GAMEPAD_Y,		0x28},
		{LEFT_THUMB_DOWNLEFT | XINPUT_GAMEPAD_B,		0x2b},

		{LEFT_THUMB_LEFT | XINPUT_GAMEPAD_A,		0x2c},
		{LEFT_THUMB_LEFT | XINPUT_GAMEPAD_X,		0x2d},
		{LEFT_THUMB_LEFT | XINPUT_GAMEPAD_Y,		0x2e},
		{LEFT_THUMB_LEFT | XINPUT_GAMEPAD_B,		0x2f},

		{LEFT_THUMB_UPLEFT | XINPUT_GAMEPAD_A,		0x30},
		{LEFT_THUMB_UPLEFT | XINPUT_GAMEPAD_X,		0x31},
		{LEFT_THUMB_UPLEFT | XINPUT_GAMEPAD_Y,		0x32},
		{LEFT_THUMB_UPLEFT | XINPUT_GAMEPAD_B,		0x33},
	};
}

// TODO: sticky modifiers

void KeyboardTranslator::UpdateKeys(INT64 state)
{
	std::map<WORD, PressedKeyInfo> currentlyPressed{};
	std::vector<WORD> toPress{};
	std::vector<WORD> toRelease{};

	for (auto& p : _bindings) {
		auto pad_keys = p.first;
		auto scan_code = p.second;
		if ((pad_keys & state) == pad_keys) {
			auto key_info_it = _pressedButtons.find(scan_code);

			if (key_info_it != _pressedButtons.end()) {
				// has been pressed for a while
				// handle auto-repeat there

				currentlyPressed.emplace(scan_code, key_info_it->second);
				PressedKeyInfo* key_info = &currentlyPressed[scan_code];

				key_info->Elapsed += 10; // TODO: replace polling time with var or smth

				// TODO don't repeat win
				if (key_info->Repeated) {
					if (key_info->Elapsed >= _autoRepeatTime) {
						toPress.push_back(scan_code);
						key_info->Elapsed = 0;
					}
				}
				else if (key_info->Elapsed >= _autoRepeatStartTime) {
					toPress.push_back(scan_code);
					key_info->Repeated = true;
					key_info->Elapsed = 0;
				}
			}
			else {
				// newly pressed
				currentlyPressed.emplace(scan_code, PressedKeyInfo{});
				toPress.push_back(scan_code);
			}
		}
	}

	for (auto& p : _pressedButtons) {
		auto scan_code = p.first;

		if (!currentlyPressed.count(scan_code)) {
			toRelease.push_back(scan_code);
		}
	}

	release(toRelease);
	press(toPress);

	_pressedButtons = currentlyPressed;
}

void KeyboardTranslator::release(std::vector<WORD> codes)
{
	for (auto& code : codes)
	{
		sendInput(code, KEYEVENTF_KEYUP);
	}
}

void KeyboardTranslator::press(std::vector<WORD> codes)
{
	for (auto& code : codes)
	{
		sendInput(code, 0);
	}
}

void KeyboardTranslator::sendInput(WORD code, DWORD flags)
{
	if (code >> 8 == 0xe0) flags |= KEYEVENTF_EXTENDEDKEY;
	INPUT input{};
	input.type = INPUT_KEYBOARD;
	input.ki.wScan = code;
	input.ki.dwFlags = KEYEVENTF_SCANCODE | flags;
	SendInput(1, &input, sizeof(INPUT));
}
