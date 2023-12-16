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

void ControllerVibrator::StikiePressed()
{
	using namespace std::chrono;
	using namespace std::chrono_literals;

	auto ui = _userIndex;
	auto x = [ui]()
		{
			auto v = XINPUT_VIBRATION{};
			ZeroMemory(&v, sizeof(XINPUT_VIBRATION));

			v.wLeftMotorSpeed = 40000;
			v.wRightMotorSpeed = 40000;
			XInputSetState(ui, &v);

			std::this_thread::sleep_for(150ms);

			v.wLeftMotorSpeed = 0;
			v.wRightMotorSpeed = 0;
			XInputSetState(0, &v);
		};

	if (_currentVibe.joinable()) _currentVibe.join();
	_currentVibe = std::thread(x);
}

void ControllerVibrator::StikieStuck()
{
	using namespace std::chrono;
	using namespace std::chrono_literals;

	auto ui = _userIndex;
	auto x = [ui]()
		{
			auto v = XINPUT_VIBRATION{};
			ZeroMemory(&v, sizeof(XINPUT_VIBRATION));

			v.wLeftMotorSpeed = 60000;
			XInputSetState(ui, &v);

			std::this_thread::sleep_for(200ms);

			v.wLeftMotorSpeed = 0;
			v.wRightMotorSpeed = 0;
			XInputSetState(0, &v);
		};

	if (_currentVibe.joinable()) _currentVibe.join();
	_currentVibe = std::thread(x);
}

void ControllerVibrator::StikieReleased()
{
	using namespace std::chrono;
	using namespace std::chrono_literals;

	auto ui = _userIndex;
	auto x = [ui]()
		{
			auto v = XINPUT_VIBRATION{};
			ZeroMemory(&v, sizeof(XINPUT_VIBRATION));

			v.wLeftMotorSpeed = 60000;
			v.wRightMotorSpeed = 60000;
			XInputSetState(ui, &v);

			std::this_thread::sleep_for(100ms);

			v.wLeftMotorSpeed = 0;
			v.wRightMotorSpeed = 0;
			XInputSetState(0, &v);

			std::this_thread::sleep_for(50ms);

			v.wLeftMotorSpeed = 60000;
			v.wRightMotorSpeed = 40000;
			XInputSetState(ui, &v);

			std::this_thread::sleep_for(100ms);

			v.wLeftMotorSpeed = 0;
			v.wRightMotorSpeed = 0;
			XInputSetState(0, &v);
		};

	if (_currentVibe.joinable()) _currentVibe.join();
	_currentVibe = std::thread(x);
}
