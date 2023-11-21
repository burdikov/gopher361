#include "KeyboardTranslator.h"

KeyboardTranslator::KeyboardTranslator()
{
	_bindingsAreScanCodes = true;

	_bindings = new std::map<INT64, BindingInfo>;
	
	(*_bindings)[RIGHT_THUMB_REST | XINPUT_GAMEPAD_DPAD_DOWN] = { .isScanCode = true, .data = 0xe05b, .typematic = false }; // left win
	(*_bindings)[XINPUT_GAMEPAD_LEFT_SHOULDER] = { true, 0x2a }; // left shift
	(*_bindings)[LEFT_THUMB_REST | XINPUT_GAMEPAD_A] = { true, 0x1c }; // enter
	(*_bindings)[LEFT_THUMB_REST | XINPUT_GAMEPAD_X] = { true, 0x0e }; // backspace
	(*_bindings)[LEFT_THUMB_REST | XINPUT_GAMEPAD_Y] = { true, 0x39 }; // space
	(*_bindings)[LEFT_THUMB_REST | XINPUT_GAMEPAD_B] = { true, 0x01 }; // esc
	(*_bindings)[LEFT_THUMB_UP | XINPUT_GAMEPAD_A] = { true, 0x10 };
	(*_bindings)[LEFT_THUMB_UP | XINPUT_GAMEPAD_X] = { true, 0x11 };
	(*_bindings)[LEFT_THUMB_UP | XINPUT_GAMEPAD_Y] = { true, 0x12 };
	(*_bindings)[LEFT_THUMB_UP | XINPUT_GAMEPAD_B] = { true, 0x13 };
	(*_bindings)[LEFT_THUMB_UPRIGHT | XINPUT_GAMEPAD_A] = { true, 0x14 };
	(*_bindings)[LEFT_THUMB_UPRIGHT | XINPUT_GAMEPAD_X] = { true, 0x15 };
	(*_bindings)[LEFT_THUMB_UPRIGHT | XINPUT_GAMEPAD_Y] = { true, 0x16 };
	(*_bindings)[LEFT_THUMB_UPRIGHT | XINPUT_GAMEPAD_B] = { true, 0x17 };
	(*_bindings)[LEFT_THUMB_RIGHT | XINPUT_GAMEPAD_A] = { true, 0x18 };
	(*_bindings)[LEFT_THUMB_RIGHT | XINPUT_GAMEPAD_X] = { true, 0x19 };
	(*_bindings)[LEFT_THUMB_RIGHT | XINPUT_GAMEPAD_Y] = { true, 0x1a };
	(*_bindings)[LEFT_THUMB_RIGHT | XINPUT_GAMEPAD_B] = { true, 0x1b };
	(*_bindings)[LEFT_THUMB_DOWNRIGHT | XINPUT_GAMEPAD_A] = { true, 0x1e };
	(*_bindings)[LEFT_THUMB_DOWNRIGHT | XINPUT_GAMEPAD_X] = { true, 0x1f };
	(*_bindings)[LEFT_THUMB_DOWNRIGHT | XINPUT_GAMEPAD_Y] = { true, 0x20 };
	(*_bindings)[LEFT_THUMB_DOWNRIGHT | XINPUT_GAMEPAD_B] = { true, 0x21 };
	(*_bindings)[LEFT_THUMB_DOWN | XINPUT_GAMEPAD_A] = { true, 0x22 };
	(*_bindings)[LEFT_THUMB_DOWN | XINPUT_GAMEPAD_X] = { true, 0x23 };
	(*_bindings)[LEFT_THUMB_DOWN | XINPUT_GAMEPAD_Y] = { true, 0x24 };
	(*_bindings)[LEFT_THUMB_DOWN | XINPUT_GAMEPAD_B] = { true, 0x25 };
	(*_bindings)[LEFT_THUMB_DOWNLEFT | XINPUT_GAMEPAD_A] = { true, 0x26 };
	(*_bindings)[LEFT_THUMB_DOWNLEFT | XINPUT_GAMEPAD_X] = { true, 0x27 };
	(*_bindings)[LEFT_THUMB_DOWNLEFT | XINPUT_GAMEPAD_Y] = { true, 0x28 };
	(*_bindings)[LEFT_THUMB_DOWNLEFT | XINPUT_GAMEPAD_B] = { true, 0x2b };
	(*_bindings)[LEFT_THUMB_LEFT | XINPUT_GAMEPAD_A] = { true, 0x2c };
	(*_bindings)[LEFT_THUMB_LEFT | XINPUT_GAMEPAD_X] = { true, 0x2d };
	(*_bindings)[LEFT_THUMB_LEFT | XINPUT_GAMEPAD_Y] = { true, 0x2e };
	(*_bindings)[LEFT_THUMB_LEFT | XINPUT_GAMEPAD_B] = { true, 0x2f };
	(*_bindings)[LEFT_THUMB_UPLEFT | XINPUT_GAMEPAD_A] = { true, 0x30 };
	(*_bindings)[LEFT_THUMB_UPLEFT | XINPUT_GAMEPAD_X] = { true, 0x31 };
	(*_bindings)[LEFT_THUMB_UPLEFT | XINPUT_GAMEPAD_Y] = { true, 0x32 };
	(*_bindings)[LEFT_THUMB_UPLEFT | XINPUT_GAMEPAD_B] = { true, 0x33 };
}

KeyboardTranslator::KeyboardTranslator(std::map<INT64, BindingInfo>* bindings, bool bindingsAreScanCodes)
{
	_bindingsAreScanCodes = bindingsAreScanCodes;
	_bindings = bindings;
}

// TODO: sticky modifiers

void KeyboardTranslator::UpdateKeys(INT64 state, int sleep_time)
{
	std::map<WORD, PressedKeyInfo> currentlyPressed{};
	std::vector<WORD> toPress{};
	std::vector<WORD> toRelease{};

	for (auto& p : *_bindings) {
		auto pad_keys = p.first;
		auto binding_info = p.second;

		if ((pad_keys & state) == pad_keys) {
			auto key_info_it = _pressedButtons.find(binding_info.data);

			if (key_info_it != _pressedButtons.end()) {
				// has been pressed for a while
				// handle auto-repeat there

				currentlyPressed.emplace(binding_info.data, key_info_it->second);

				if (!binding_info.typematic) continue;  // skip auto-repeat

				PressedKeyInfo* key_info = &currentlyPressed[binding_info.data];

				key_info->Elapsed += sleep_time;

				if (key_info->Repeated) {
					if (key_info->Elapsed >= _autoRepeatTime) {
						toPress.push_back(binding_info.data);
						key_info->Elapsed = 0;
					}
				}
				else if (key_info->Elapsed >= _autoRepeatStartTime) {
					toPress.push_back(binding_info.data);
					key_info->Repeated = true;
					key_info->Elapsed = 0;
				}
			}
			else {
				// newly pressed
				currentlyPressed.emplace(binding_info.data, PressedKeyInfo{});
				toPress.push_back(binding_info.data);
			}
		}
	}

	for (auto& p : _pressedButtons) {  // TODO: rename scan_code cuz it can be vk code too now
		auto scan_code = p.first;

		if (!currentlyPressed.count(scan_code)) {
			toRelease.push_back(scan_code);
		}
	}

	release(toRelease);
	press(toPress);

	_pressedButtons = currentlyPressed;
}

void KeyboardTranslator::release(std::vector<WORD> data)
{
	for (auto& code : data)
	{
		sendInput(code, KEYEVENTF_KEYUP);
	}
}

void KeyboardTranslator::press(std::vector<WORD> data)
{
	for (auto& code : data)
	{
		sendInput(code, 0);
	}
}

void KeyboardTranslator::sendInput(WORD data, DWORD flags)
{
	INPUT input{};
	input.type = INPUT_KEYBOARD;
	input.ki.dwFlags |= flags;

	if (_bindingsAreScanCodes) {
		if (data >> 8 == 0xe0) flags |= KEYEVENTF_EXTENDEDKEY;  // I can move it to BindingInfo, but do I... need to?
		input.ki.wScan = data;
		input.ki.dwFlags = KEYEVENTF_SCANCODE | flags;
	}
	else {
		if (data <= 2 || data >= 4 && data <= 6) {
			// that's a mouse VK, change input type to mouse
			// cuz just virtual keys are ignored :_)
			input = INPUT{};
			input.type = INPUT_MOUSE;

			auto released = (flags & KEYEVENTF_KEYUP) == KEYEVENTF_KEYUP;

			switch (data) {
			case VK_LBUTTON:
				input.mi.dwFlags |= released ? MOUSEEVENTF_LEFTUP : MOUSEEVENTF_LEFTDOWN;
				break;
			case VK_RBUTTON:
				input.mi.dwFlags |= released ? MOUSEEVENTF_RIGHTUP : MOUSEEVENTF_RIGHTDOWN;
				break;
			case VK_MBUTTON:
				input.mi.dwFlags |= released ? MOUSEEVENTF_MIDDLEUP : MOUSEEVENTF_MIDDLEDOWN;
				break;
			case VK_XBUTTON1:
				input.mi.mouseData = XBUTTON1;
				input.mi.dwFlags |= released ? MOUSEEVENTF_XUP : MOUSEEVENTF_XDOWN;
				break;
			case VK_XBUTTON2:
				input.mi.mouseData = XBUTTON2;
				input.mi.dwFlags |= released ? MOUSEEVENTF_XUP : MOUSEEVENTF_XDOWN;
				break;
			}
		}
		else {
			input.ki.wVk = data;
		}
	}

	SendInput(1, &input, sizeof(INPUT));
}
