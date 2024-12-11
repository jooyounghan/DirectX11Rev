#pragma once
#include <windows.h>
#include <cassert>

#define D3D11_BUFFER_ALIGN 16
#define BUFFER_ALIGN_CHECK(Size) assert(Size % D3D11_BUFFER_ALIGN == 0);

#define AutoZeroMemory(Object) ZeroMemory(&Object, sizeof(decltype(Object)))
#define AssertIfFailed(IsFailed) { bool Result = FAILED(IsFailed); assert(!Result); }