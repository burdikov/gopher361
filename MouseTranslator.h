#pragma once

#include "Windows.h"
#include <Xinput.h>
#include <math.h>
class MouseTranslator
{
private:
	short _sens = 1600;
	short _dead_zone = 3000;
	
	// pixel accumulators
	double _dlx_acc, _dly_acc;
	double _drx_acc, _dry_acc;
public:
	void MovePointer(XINPUT_STATE state, int sleep_time);
	double* Scroll(XINPUT_STATE state, int sleep_time);
};

