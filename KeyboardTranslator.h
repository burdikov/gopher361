#pragma once
#include <windows.h>
#include <map>
#include <unordered_map>
#include <unordered_set>

#include "XInputController.h"

struct BindingInfo {
	// Scan code or virtual key
	WORD data;
	// If true (default), data contains scan code of the key
	bool isScanCode = true;
	// If true, key is not input right away, instead it's prepended to next non-sticky key
	bool isSticky = false;
	// true (default), if key should be repeated
	bool isTypematic = true;
};

class KeyboardTranslator
{
private:
	struct PressedKeyInfo
	{
		PressedKeyInfo() : Repeated(false), Elapsed(0), Sticky(false) {};
		PressedKeyInfo(BindingInfo binding_info);
		// see BindingInfo.isSticky
		bool Sticky;
		// True, if auto-repeat has started for this key.
		bool Repeated;
		// Time in milliseconds since key has been pressed. If Repeated is true, this is the time since last repetition.
		int Elapsed;
	};

	XInputController* _controller;

	std::map<INT64, BindingInfo>* _bindings;

	std::map<WORD, PressedKeyInfo> _pressedButtons;

	std::unordered_set<WORD> _onetimeStickies;
	std::unordered_set<WORD> _stuckStickies;

	int _autoRepeatStartTime = 400;
	int _autoRepeatTime = 20;

	int _stikieStuckingTime = 400;

	bool _bindingsAreScanCodes;
public:
	KeyboardTranslator();
	KeyboardTranslator(XInputController* controller);
	KeyboardTranslator(std::map<INT64, BindingInfo>* bindings, bool bindingsAreScanCodes = false);
	KeyboardTranslator(XInputController* controller, std::map<INT64, BindingInfo>* bindings, bool bindingsAreScanCodes = false) :
		_controller(controller), _bindings(bindings), _bindingsAreScanCodes(bindingsAreScanCodes) {};

	void UpdateKeys(INT64 state, int sleep_time);
private:
	void release(std::vector<WORD> codes);
	void press(std::vector<WORD> codes);
	void sendInput(WORD code, DWORD flags);
};