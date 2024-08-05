#pragma once
#include <windows.h>
#include <DirectXMath.h>

namespace App
{
	extern UINT GWidth;
	extern UINT GHeight;
}

namespace Direction
{
	enum EDirection : size_t
	{
		Right,			// x
		Up,				// y
		Forward,		// z
		NumDirection
	};

	extern DirectX::XMVECTOR GDefaultForward;
	extern DirectX::XMVECTOR GDefaultUp;
	extern DirectX::XMVECTOR GDefaultRight;
}

struct ImVec4;
namespace Color
{
	extern ImVec4 GBlack;
	extern ImVec4 GWhite;
	extern ImVec4 GDarkGray;
	extern ImVec4 GLightGray;
	extern ImVec4 GDarkBrown;
	extern ImVec4 GMediumBrown;
	extern ImVec4 GLightBrown;
}

namespace DragDrop
{
	extern const char* GAsset;
}