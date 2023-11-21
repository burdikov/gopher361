#pragma once

#include <Windows.h>
#include <Xinput.h>
#include <chrono>
#include <thread>

class ControllerVibrator
{
private:
	int _userIndex;

	std::thread _currentVibe;
public:
	ControllerVibrator() : _userIndex{} {};
	ControllerVibrator(int userIndex) : _userIndex(userIndex) {};

	void ProfileSwitch();
	void Enable();
};

