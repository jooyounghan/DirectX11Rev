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
		KeyCommand�� ���� shared_ptr�� static���� ������ �ְ�,
		�̸� ��Ÿ�ӿ� ��ü�ϸ鼭 �� �� �ֵ��� ����!
	*/
};

