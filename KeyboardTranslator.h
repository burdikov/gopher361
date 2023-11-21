#pragma once
#include <windows.h>
#include <map>
#include <unordered_map>

#include "XInputController.h"

struct BindingInfo {
	/*BindingInfo() : isScanCode{}, data{}, typematic(true) {}
	BindingInfo(bool isScanCode, WORD data) : isScanCode(isScanCode), data(data) {}
	
	BindingInfo(bool isScanCode, WORD data, bool typematic) : 
		isScanCode(isScanCode), 
		data(data), 
		typematic(typematic) 
	{}*/


	bool isScanCode;
	// scan code or virtual key
	WORD data; 
	// bool isSticky; // for future
	// true, if key should be repeated
	bool typematic;
};

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

	std::map<INT64, BindingInfo>* _bindings;

	std::map<WORD, PressedKeyInfo> _pressedButtons;

	int _autoRepeatStartTime = 400;
	int _autoRepeatTime = 20;

	bool _bindingsAreScanCodes;
public:
	KeyboardTranslator();
	KeyboardTranslator(std::map<INT64, BindingInfo>* bindings, bool bindingsAreScanCodes = false);

	void UpdateKeys(INT64 state, int sleep_time);
private:
	void release(std::vector<WORD> codes);
	void press(std::vector<WORD> codes);
	void sendInput(WORD code, DWORD flags);
};