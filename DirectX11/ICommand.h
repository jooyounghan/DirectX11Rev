#pragma once
#include "DefineKey.h"

class IKeyCommand
{
	virtual void Execute(EKeyAction KeyAction) = 0;
};

