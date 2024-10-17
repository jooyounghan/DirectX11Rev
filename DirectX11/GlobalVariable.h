#pragma once
#include <windows.h>
#include "directxmath/DirectXMath.h"

class PortfolioApp;
class GraphicsPipeline;
class PSOManager;
class InputEventManager;

namespace App
{
	extern UINT						GWidth;
	extern UINT						GHeight;
	extern PortfolioApp*			GApp;
	extern GraphicsPipeline*		GGraphicPipeline;
	extern PSOManager*				GPSOManager;
	extern InputEventManager*		GInputEventManager;
}

namespace Performance
{
	extern size_t		GTotalIndexCount;
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

	extern const ImVec4 GHilighted;
}

namespace NodeColor
{
	extern const unsigned int VariableNodeBaseColor;
	extern const unsigned int VariableNodeHilightedColor;
	extern const unsigned int VariablePortBaseColor;
	extern const unsigned int VariablePortHilightedColor;

	extern const unsigned int FlowNodeBaseColor;
	extern const unsigned int FlowNodeHilightedColor;
	extern const unsigned int FlowPortBaseColor;
	extern const unsigned int FlowPortHilightedColor;
}

namespace UISize
{
	extern const ImVec2 FileSize;
	extern const ImVec2 WindowSize;
}

namespace DragDrop
{
	extern const char* GAsset;
}