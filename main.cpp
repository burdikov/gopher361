// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <iostream>
#include <cmath>
#include <ctgmath>
#include <map>
#include "XInputController.h"
#include "KeyboardTranslator.h"
#include "MouseTranslator.h"

#pragma comment(lib, "Xinput.lib")

int main()
{
    XInputController controller(0);
	KeyboardTranslator kbTranslator{};
	MouseTranslator mTranslator{};

	int counter = 0;

	int sleep_time = 10;

	while (true)
	{
		controller.UpdateState();
		auto state = controller.GetState();

		mTranslator.MovePointer(state, sleep_time);
		double * a = mTranslator.Scroll(state, sleep_time);

		if (a != nullptr) {
			for (int i = 0; i < 2; i++) {
				printf("%lf ", a[i]);
			}
		}

		//auto state = controller.GetLongState();
		
		//translator.UpdateKeys(state); // state = 16842752

		Sleep(sleep_time);
		std::cout << "\x1b[2K";
		std::cout << "\r";
	}
}