// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <iostream>
#include <cmath>
#include <map>
#include "XInputController.h"
#include "KeyboardTranslator.h"
#include "MouseTranslator.h"
#include <chrono>
#include <thread>

#pragma comment(lib, "Xinput.lib")

#include "Profile.h"


// TODO: profile speed-dial
int main()
{
	using namespace std::chrono;
	using namespace std::chrono_literals;
	using namespace std::this_thread;

	XInputController controller(0);

	controller.Vibrator.Enable();

	// default kb settings for now
	Profile kb_no_mouse = Profile(nullptr, new KeyboardTranslator(&controller));

	// kb with mouse clicks
	std::map<INT64, BindingInfo> mkb_bindings{};
	mkb_bindings[XINPUT_GAMEPAD_A] = { .data = VK_LBUTTON, .isScanCode = false, .isTypematic = false };
	mkb_bindings[XINPUT_GAMEPAD_X] = { .data = VK_RBUTTON, .isScanCode = false, .isTypematic = false };
	auto mouse_kb_translator = new KeyboardTranslator(&mkb_bindings);

	Profile mkb = Profile(new MouseTranslator(), mouse_kb_translator);

	std::vector<Profile> profiles = { mkb, kb_no_mouse };

	int sleep_time = 10;
	int current_profile = 0;


	// profile switch timeout
	auto prof_sw_timeout = 300ms;
	// last profile switch timestamp
	auto last_prof_sw_ts = steady_clock::now();

	// profile switch button
	auto prof_sw_btn = XINPUT_GAMEPAD_RIGHT_THUMB;

	while (true)
	{
		controller.UpdateState();
		auto state = controller.GetState();
		auto long_state = controller.GetLongState(); // TODO: zero out sticks

		if ((state.Gamepad.wButtons & prof_sw_btn) == prof_sw_btn && steady_clock::now() - last_prof_sw_ts > prof_sw_timeout) {
			++current_profile %= profiles.size();

			last_prof_sw_ts = steady_clock::now();

			controller.Vibrator.ProfileSwitch();

			continue;
		}
		
		auto cp = &profiles[current_profile];
		if (cp->mTranslator != nullptr) {
			// sticks are mouse
			cp->mTranslator->MovePointer(state, sleep_time);
			cp->mTranslator->Scroll(state, sleep_time);
		}

		cp->kbTranslator->UpdateKeys(long_state, sleep_time);

		Sleep(sleep_time);
		std::cout << "\x1b[2K";
		std::cout << "\r";
	}
}

 
 
 
 




 