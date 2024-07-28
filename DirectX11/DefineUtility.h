#pragma once
#include <windows.h>
#include <stdio.h>
#include <cassert>

#define D3D11_BUFFER_ALIGN 16
#define BUFFER_ALIGN_CHECK(Size) assert(Size % D3D11_BUFFER_ALIGN == 0);

#define AutoZeroMemory(PlacableObject) ZeroMemory(&PlacableObject, sizeof(decltype(PlacableObject)))
#define AutoZeroArrayMemory(Array) ZeroMemory(Array, sizeof(decltype(Array)))

#define AssertIfFailed(IsFailed) { bool Result = FAILED(IsFailed); assert(!Result); }

template <typename T, UINT N>
constexpr UINT GetArraySize(T(&)[N]) noexcept 
{
	return N;
}
	