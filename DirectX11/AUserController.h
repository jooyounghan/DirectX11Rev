#pragma once
#include "AController.h"

#include <memory>

class AUserController : public AController
{
public:
	AUserController();

protected:
	bool KeyPressedStates[KeyBoardInputCount];

protected:
	
	/*
		KeyCommand에 대해 shared_ptr로 static으로 가지고 있고,
		이를 런타임에 교체하면서 쓸 수 있도록 설정!
	*/
};

