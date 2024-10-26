#pragma once
#include <windows.h>
#include "directxmath/DirectXMath.h"

class PortfolioApp;
class GraphicsPipeline;
class PSOManager;
class InputEventManager;
class UploadableBufferManager;

namespace App
{
	extern UINT						GWidth;
	extern UINT						GHeight;
	extern PortfolioApp*			GApp;
	extern GraphicsPipeline*		GGraphicPipeline;
	extern PSOManager*				GPSOManager;
	extern InputEventManager*		GInputEventManager;
	extern UploadableBufferManager*	GUploadableBufferManager;
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