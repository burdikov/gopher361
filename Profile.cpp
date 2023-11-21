#include "Profile.h"

Profile::Profile()
{
	mTranslator = nullptr;
	kbTranslator = nullptr;
}

Profile::Profile(MouseTranslator* mt, KeyboardTranslator* kbt)
{
	mTranslator = mt;
	kbTranslator = kbt;
}
