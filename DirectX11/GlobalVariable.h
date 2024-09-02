#pragma once
#include <windows.h>
#include <DirectXMath.h>

class PortfolioApp;
class GraphicsPipeline;
class InputEventManager;

namespace App
{
	extern UINT GWidth;
	extern UINT GHeight;
	extern PortfolioApp* GApp;
	extern GraphicsPipeline* GGraphicPipeline;
	extern InputEventManager* GInputEventManager;
}

namespace Direction
{
	constexpr DirectX::XMVECTOR GDefaultForward = DirectX::XMVECTOR{ 0.f, 0.f, 1.f, 0.f };
	constexpr DirectX::XMVECTOR GDefaultUp = DirectX::XMVECTOR{ 0.f, 1.f, 0.f, 0.f };
	constexpr DirectX::XMVECTOR GDefaultRight = DirectX::XMVECTOR{ 1.f, 0.f, 0.f, 0.f };
}

struct ImVec2;
struct ImVec4;

namespace UIColor
{
	extern const ImVec4 GBlack;
	extern const ImVec4 GWhite;
	extern const ImVec4 GDarkGray;
	extern const ImVec4 GLightGray;
	extern const ImVec4 GDarkBrown;
	extern const ImVec4 GMediumBrown;
	extern const ImVec4 GLightBrown;
}

namespace UISize
{
	extern const ImVec2 FileSize;
}

namespace DragDrop
{
	extern const char* GAsset;
}