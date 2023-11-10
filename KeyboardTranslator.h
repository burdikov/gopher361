#pragma once
#include <windows.h>
#include <unordered_map>

class KeyboardTranslator
{
private:
	 std::unordered_map<DWORD, bool> _pressedButtons;
	 std::unordered_map<DWORD, bool> _pressedButtonsPrevCycle;
	 std::unordered_map<DWORD, bool> _releasedButtons;
};

