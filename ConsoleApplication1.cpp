// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <iostream>
#include <cmath>
#include <ctgmath>

#include "XInputController.h"

#pragma comment(lib, "Xinput.lib")


#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE 2500

int main()
{
    XInputController controller(0);

	int counter = 0;
	while (true)
	{
		controller.UpdateState();

		printf("%i eighth", controller.GetLeftStickDirection());

		Sleep(10);
		std::cout << "\x1b[2K";
		std::cout << "\r";
	}
}