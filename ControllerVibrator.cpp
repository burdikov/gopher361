#include "ControllerVibrator.h"

void ControllerVibrator::ProfileSwitch()
{
	using namespace std::chrono;
	using namespace std::chrono_literals;

	auto ui = _userIndex;
	auto x = [ui]()
		{
			auto v = XINPUT_VIBRATION{};
			ZeroMemory(&v, sizeof(XINPUT_VIBRATION));

			v.wRightMotorSpeed = 32000;
			XInputSetState(ui, &v);

			std::this_thread::sleep_for(135ms);

			v.wLeftMotorSpeed = 0;
			v.wRightMotorSpeed = 0;
			XInputSetState(0, &v);
		};

	if (_currentVibe.joinable()) _currentVibe.join();
	_currentVibe = std::thread(x);
}

void ControllerVibrator::Enable()
{
	using namespace std::chrono;
	using namespace std::chrono_literals;

	auto ui = _userIndex;
	auto x = [ui]()
		{
			auto v = XINPUT_VIBRATION{};
			ZeroMemory(&v, sizeof(XINPUT_VIBRATION));

			v.wLeftMotorSpeed = 64000;
			XInputSetState(ui, &v);
			
			std::this_thread::sleep_for(200ms);

			v.wLeftMotorSpeed = 0;
			v.wRightMotorSpeed = 0;
			XInputSetState(0, &v);
		};

	if (_currentVibe.joinable()) _currentVibe.join();
	_currentVibe = std::thread(x);
}
