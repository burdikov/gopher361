#pragma once
#include <windows.h>
#include <map>
#include <unordered_map>

#include "XInputController.h"

class KeyboardTranslator
{
private:
	struct PressedKeyInfo
	{
		// True, if auto-repeat has started for this key.
		bool Repeated;
		// Time in milliseconds since key has been pressed. If Repeated is true, this is the time since last repetition.
		int Elapsed;
	};

	std::map<INT64, WORD> _bindings;

	std::map<WORD, PressedKeyInfo> _pressedButtons;

	int _autoRepeatStartTime = 400;
	int _autoRepeatTime = 20;
public:
	KeyboardTranslator();

	void UpdateKeys(LONG64 state);
private:
	void release(std::vector<WORD> codes);
	void press(std::vector<WORD> codes);
	void sendInput(WORD code, DWORD flags);
};


