#include "MouseTranslator.h"

void MouseTranslator::MovePointer(XINPUT_STATE state, int sleep_time)
{
	POINT p{};
	GetCursorPos(&p);

	auto gx = state.Gamepad.sThumbLX;
	auto gy = state.Gamepad.sThumbLY;

	// distance from origin
	auto magnitude = sqrt(gx * gx + gy * gy);

	// normalized value
	double norm_gx{}, norm_gy{}, norm_mag{};
	if (magnitude > _dead_zone) {
		if (magnitude > 32767) magnitude = 32767;
		norm_gx = gx / magnitude;
		norm_gy = gy / magnitude;

		norm_mag = (magnitude - _dead_zone) / (32767 - _dead_zone);
	} // else return

	// add current delta to accumulators
	_dlx_acc += _sens * norm_mag * norm_mag * norm_mag * norm_gx * ((double)sleep_time / 1000);
	_dly_acc += _sens * norm_mag * norm_mag * norm_mag * norm_gy * ((double)sleep_time / 1000);

	// take integer parts from accumulators and move cursor that much pixels
	p.x += (int)_dlx_acc;
	p.y -= (int)_dly_acc;

	// subtract those pixels from accumulators
	_dlx_acc -= (int)_dlx_acc;
	_dly_acc -= (int)_dly_acc;

	SetCursorPos(p.x, p.y);
}

// apparently, only one type of scroll can happen at a time
// so we will choose and send only one type for smoooooooothiness
double* MouseTranslator::Scroll(XINPUT_STATE state, int sleep_time)
{
	static double res[2];

	auto gx = state.Gamepad.sThumbRX;
	auto gy = state.Gamepad.sThumbRY;

	// distance from origin
	auto magnitude = sqrt(gx * gx + gy * gy);
	if (magnitude > MAXSHORT) magnitude = MAXSHORT;

	INPUT input{};
	input.type = INPUT_MOUSE;
	double norm_gx{}, norm_gy{}, norm_mag{};
	norm_mag = (magnitude - _dead_zone) / (MAXSHORT - _dead_zone);
	if (magnitude < _dead_zone) {
		_drx_acc = _dry_acc = 0;
		return res;
	}
	else if (abs((int)gy) >= abs((int)gx)) {
		// vertical scroll
		norm_gy = gy / magnitude;
		// add current delta to accumulator
		_dry_acc += 0.5 * norm_gy * norm_mag * norm_mag * abs(gy) * ((double)sleep_time / 1000);
	
		input.mi.mouseData = (int)_dry_acc;
		input.mi.dwFlags = MOUSEEVENTF_WHEEL;

		res[0] = _dry_acc;
	
		// subtract sent pixels from accumulators
		_dry_acc -= (int)_dry_acc;
	}
	else {
		// horizontal scroll
		norm_gx = gx / magnitude;
		_drx_acc += 0.3 * norm_gx * norm_mag * norm_mag * abs(gx) * ((double)sleep_time / 1000);

		input.mi.mouseData = (int)_drx_acc;
		input.mi.dwFlags = MOUSEEVENTF_HWHEEL;
	
		res[1] = _drx_acc;

		_drx_acc -= (int)_drx_acc;
	}

	SendInput(1, &input, sizeof(INPUT));

	return res;
}
