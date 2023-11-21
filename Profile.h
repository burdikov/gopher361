#pragma once
#include "MouseTranslator.h"
#include "KeyboardTranslator.h"
class Profile
{
public:
	Profile();
	Profile(MouseTranslator* mt, KeyboardTranslator* kbt);

	MouseTranslator* mTranslator;
	KeyboardTranslator* kbTranslator;
};

