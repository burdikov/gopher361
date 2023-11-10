// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <iostream>
#include <cmath>
#include <ctgmath>
#include <map>
#include "XInputController.h"

#pragma comment(lib, "Xinput.lib")

int main()
{
    XInputController controller(0);

	int counter = 0;
	while (true)
	{
		controller.UpdateState();

		std::map<DWORD, WORD> bindings = {
			{LEFT_THUMB_UP | XINPUT_GAMEPAD_A, 0x10},
			{LEFT_THUMB_DOWN | XINPUT_GAMEPAD_A, 0x11},
			{LEFT_THUMB_UP | XINPUT_GAMEPAD_B, 0x12},
		};

		auto state = controller.GetLongState();

		for (const auto& p : bindings) {
			if ((p.first & state) == p.first) {
				printf("%i ", p.second);
				INPUT input{};
				input.type = INPUT_KEYBOARD;
				input.ki.wScan = p.second;
				input.ki.dwFlags = KEYEVENTF_SCANCODE;
				SendInput(1, &input, sizeof(INPUT));
			}

			Sleep(3000);

			INPUT input{};
			input.type = INPUT_KEYBOARD;
			input.ki.wScan = p.second;
			input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
			SendInput(1, &input, sizeof(INPUT));

			INPUT input1{};
			input1.type = INPUT_KEYBOARD;
			input1.ki.wScan = 0x1c;
			input1.ki.dwFlags = KEYEVENTF_SCANCODE;
			SendInput(1, &input1, sizeof(INPUT));

			Sleep(10);

			INPUT input2{};
			input2.type = INPUT_KEYBOARD;
			input2.ki.wScan = 0x1c;
			input2.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
			SendInput(1, &input2, sizeof(INPUT));
		}
		
		Sleep(3000);
		std::cout << "\x1b[2K";
		std::cout << "\r";
	}
}