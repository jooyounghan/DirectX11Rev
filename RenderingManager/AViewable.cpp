
#include "AViewable.h"
#include "DirectXUtilities.h"

AViewable::AViewable(
	const float& width, 
	const float& height, 
	const float& topLeftX, 
	const float& topLeftY, 
	const float& minDepth, 
	const float& maxDepth
)
{
	AutoZeroMemory(m_viewport);
	m_viewport.Width = width;
	m_viewport.Height = height;
	m_viewport.TopLeftX = topLeftX;
	m_viewport.TopLeftY = topLeftY;
	m_viewport.MinDepth = minDepth;
	m_viewport.MaxDepth = maxDepth;
}
