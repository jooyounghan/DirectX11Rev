#include "GlobalVariable.h"

using namespace DirectX;

UINT App::GWidth = 0;
UINT App::GHeight = 0;

XMVECTOR Direction::GDefaultForward = XMVectorSet(0.f, 0.f, 1.f, 0.f);
XMVECTOR Direction::GDefaultUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
XMVECTOR Direction::GDefaultRight = XMVectorSet(1.f, 0.f, 0.f, 0.f);
