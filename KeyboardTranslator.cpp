#include "KeyboardTranslator.h"

KeyboardTranslator::KeyboardTranslator()
{
	_bindingsAreScanCodes = true;

	_onetimeStickies = std::unordered_set<WORD>();
	_stuckStickies = std::unordered_set<WORD>();
	_bindings = new std::map<INT64, BindingInfo>;

	(*_bindings)[RIGHT_THUMB_REST | XINPUT_GAMEPAD_DPAD_DOWN] = { .data = 0xe05b, .isTypematic = false }; // left win
	(*_bindings)[XINPUT_GAMEPAD_LEFT_SHOULDER] = { .data = 0x2a, .isSticky = true }; // left shift
	(*_bindings)[LEFT_THUMB_REST | XINPUT_GAMEPAD_A] = { 0x1c }; // enter
	(*_bindings)[LEFT_THUMB_REST | XINPUT_GAMEPAD_X] = { 0x0e }; // backspace
	(*_bindings)[LEFT_THUMB_REST | XINPUT_GAMEPAD_Y] = { 0x39 }; // space
	(*_bindings)[LEFT_THUMB_REST | XINPUT_GAMEPAD_B] = { 0x01 }; // esc
	(*_bindings)[LEFT_THUMB_UP | XINPUT_GAMEPAD_A] = { 0x10 };
	(*_bindings)[LEFT_THUMB_UP | XINPUT_GAMEPAD_X] = { 0x11 };
	(*_bindings)[LEFT_THUMB_UP | XINPUT_GAMEPAD_Y] = { 0x12 };
	(*_bindings)[LEFT_THUMB_UP | XINPUT_GAMEPAD_B] = { 0x13 };
	(*_bindings)[LEFT_THUMB_UPRIGHT | XINPUT_GAMEPAD_A] = { 0x14 };
	(*_bindings)[LEFT_THUMB_UPRIGHT | XINPUT_GAMEPAD_X] = { 0x15 };
	(*_bindings)[LEFT_THUMB_UPRIGHT | XINPUT_GAMEPAD_Y] = { 0x16 };
	(*_bindings)[LEFT_THUMB_UPRIGHT | XINPUT_GAMEPAD_B] = { 0x17 };
	(*_bindings)[LEFT_THUMB_RIGHT | XINPUT_GAMEPAD_A] = { 0x18 };
	(*_bindings)[LEFT_THUMB_RIGHT | XINPUT_GAMEPAD_X] = { 0x19 };
	(*_bindings)[LEFT_THUMB_RIGHT | XINPUT_GAMEPAD_Y] = { 0x1a };
	(*_bindings)[LEFT_THUMB_RIGHT | XINPUT_GAMEPAD_B] = { 0x1b };
	(*_bindings)[LEFT_THUMB_DOWNRIGHT | XINPUT_GAMEPAD_A] = { 0x1e };
	(*_bindings)[LEFT_THUMB_DOWNRIGHT | XINPUT_GAMEPAD_X] = { 0x1f };
	(*_bindings)[LEFT_THUMB_DOWNRIGHT | XINPUT_GAMEPAD_Y] = { 0x20 };
	(*_bindings)[LEFT_THUMB_DOWNRIGHT | XINPUT_GAMEPAD_B] = { 0x21 };
	(*_bindings)[LEFT_THUMB_DOWN | XINPUT_GAMEPAD_A] = { 0x22 };
	(*_bindings)[LEFT_THUMB_DOWN | XINPUT_GAMEPAD_X] = { 0x23 };
	(*_bindings)[LEFT_THUMB_DOWN | XINPUT_GAMEPAD_Y] = { 0x24 };
	(*_bindings)[LEFT_THUMB_DOWN | XINPUT_GAMEPAD_B] = { 0x25 };
	(*_bindings)[LEFT_THUMB_DOWNLEFT | XINPUT_GAMEPAD_A] = { 0x26 };
	(*_bindings)[LEFT_THUMB_DOWNLEFT | XINPUT_GAMEPAD_X] = { 0x27 };
	(*_bindings)[LEFT_THUMB_DOWNLEFT | XINPUT_GAMEPAD_Y] = { 0x28 };
	(*_bindings)[LEFT_THUMB_DOWNLEFT | XINPUT_GAMEPAD_B] = { 0x2b };
	(*_bindings)[LEFT_THUMB_LEFT | XINPUT_GAMEPAD_A] = { 0x2c };
	(*_bindings)[LEFT_THUMB_LEFT | XINPUT_GAMEPAD_X] = { 0x2d };
	(*_bindings)[LEFT_THUMB_LEFT | XINPUT_GAMEPAD_Y] = { 0x2e };
	(*_bindings)[LEFT_THUMB_LEFT | XINPUT_GAMEPAD_B] = { 0x2f };
	(*_bindings)[LEFT_THUMB_UPLEFT | XINPUT_GAMEPAD_A] = { 0x30 };
	(*_bindings)[LEFT_THUMB_UPLEFT | XINPUT_GAMEPAD_X] = { 0x31 };
	(*_bindings)[LEFT_THUMB_UPLEFT | XINPUT_GAMEPAD_Y] = { 0x32 };
	(*_bindings)[LEFT_THUMB_UPLEFT | XINPUT_GAMEPAD_B] = { 0x33 };
}

KeyboardTranslator::KeyboardTranslator(XInputController* controller) : KeyboardTranslator()
{
	_controller = controller;
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

				currentlyPressed.emplace(binding_info.data, key_info_it->second);

				PressedKeyInfo* key_info = &currentlyPressed[binding_info.data];

				key_info->Elapsed += sleep_time;

				if (binding_info.isTypematic && !binding_info.isSticky) {
					// handle auto-repeat
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

				if (binding_info.isSticky && key_info->Elapsed >= _stikieStuckingTime && _stuckStickies.count(binding_info.data) == 0) {
					_onetimeStickies.erase(binding_info.data);
					_stuckStickies.insert(binding_info.data);

					_controller->Vibrator.StikieStuck(); // TODO key can be stuck after being reset without re-pressing (hi, p**in) it, I don't like it
				}
			}
			else {
				// newly pressed
				currentlyPressed.emplace(binding_info.data, PressedKeyInfo(binding_info));

				if (binding_info.isSticky) {
					if (_onetimeStickies.count(binding_info.data) > 0 || _stuckStickies.count(binding_info.data) > 0) {
						// 
						_onetimeStickies.erase(binding_info.data);
						_stuckStickies.erase(binding_info.data);

						_controller->Vibrator.StikieReleased();
					}
					else {
						_onetimeStickies.insert(binding_info.data);

						_controller->Vibrator.StikiePressed();
					}
				}
				else toPress.push_back(binding_info.data);
			}
		}
	}

	for (auto& p : _pressedButtons) {  // TODO: rename scan_code cuz it can be vk code too now
		auto data = p.first;

		auto key_info_it = currentlyPressed.find(data);

		if (key_info_it != currentlyPressed.end() && !key_info_it->second.Sticky) {
			toRelease.push_back(data);
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

INPUT prepScanCodeInput(WORD code, DWORD flags = 0) {
	INPUT i{};
	i.type = INPUT_KEYBOARD;

	if (code >> 8 == 0xe0) flags |= KEYEVENTF_EXTENDEDKEY;
	i.ki.wScan = code;
	i.ki.dwFlags = KEYEVENTF_SCANCODE | flags;

	return i;
}

INPUT prepVkInput(WORD vk, DWORD flags = 0) {
	INPUT i{};
	i.type = INPUT_KEYBOARD;
	i.ki.dwFlags |= flags;
	i.ki.wVk = vk;

	return i;
}

// converts mouse VK to mouse input
INPUT prepMouseInput(WORD vk, DWORD flags = 0) {
	INPUT i{};
	i.type = INPUT_MOUSE;

	bool released = (flags & KEYEVENTF_KEYUP) == KEYEVENTF_KEYUP;

	switch (vk) {
	case VK_LBUTTON:
		i.mi.dwFlags |= released ? MOUSEEVENTF_LEFTUP : MOUSEEVENTF_LEFTDOWN;
		break;
	case VK_RBUTTON:
		i.mi.dwFlags |= released ? MOUSEEVENTF_RIGHTUP : MOUSEEVENTF_RIGHTDOWN;
		break;
	case VK_MBUTTON:
		i.mi.dwFlags |= released ? MOUSEEVENTF_MIDDLEUP : MOUSEEVENTF_MIDDLEDOWN;
		break;
	case VK_XBUTTON1:
		i.mi.mouseData = XBUTTON1;
		i.mi.dwFlags |= released ? MOUSEEVENTF_XUP : MOUSEEVENTF_XDOWN;
		break;
	case VK_XBUTTON2:
		i.mi.mouseData = XBUTTON2;
		i.mi.dwFlags |= released ? MOUSEEVENTF_XUP : MOUSEEVENTF_XDOWN;
		break;
	}

	return i;
}

void KeyboardTranslator::sendInput(WORD data, DWORD flags)
{
	auto inputs_no = (_onetimeStickies.size() + _stuckStickies.size()) * 2 + 1;
	INPUT* inputs = new INPUT[inputs_no];

	int i = 0;
	for (auto w : _stuckStickies) {
		inputs[i] = _bindingsAreScanCodes ? prepScanCodeInput(w) : prepVkInput(w);
		i++;
	}

	for (auto w : _onetimeStickies) {
		inputs[i] = _bindingsAreScanCodes ? prepScanCodeInput(w) : prepVkInput(w);
		i++;
	}

	if (_bindingsAreScanCodes) {
		inputs[i] = prepScanCodeInput(data, flags);
	}
	else {
		if (data <= 2 || data >= 4 && data <= 6) {
			// that's a mouse VK, change input type to mouse, because virtual keys are just ignored :_)
			inputs[i] = prepMouseInput(data, flags);
		}
		else {
			inputs[i] = prepVkInput(data, flags);
		}
	}

	i++;
	for (auto w : _stuckStickies) {
		inputs[i] = _bindingsAreScanCodes ? prepScanCodeInput(w, KEYEVENTF_KEYUP) : prepVkInput(w, KEYEVENTF_KEYUP);
		i++;
	}

	for (auto w : _onetimeStickies) {
		inputs[i] = _bindingsAreScanCodes ? prepScanCodeInput(w, KEYEVENTF_KEYUP) : prepVkInput(w, KEYEVENTF_KEYUP);
		i++;
	}

	_onetimeStickies.clear();

	SendInput(inputs_no, inputs, sizeof(INPUT));
}

KeyboardTranslator::PressedKeyInfo::PressedKeyInfo(BindingInfo binding_info) : PressedKeyInfo()
{
	Sticky = binding_info.isSticky;
}
