#include "DrawElementManager.h"

DrawElementManager::~DrawElementManager()
{
	for (ADrawElement* drawElement : m_drawElements)
	{
		delete drawElement;
	}
}

void DrawElementManager::AddDrawElement(ADrawElement* drawElement) 
{
	m_drawElements.push_back(drawElement); 
}

void DrawElementManager::RemoveDrawElement(ADrawElement* drawElement)
{
	m_drawElements.erase(
		std::remove(m_drawElements.begin(), m_drawElements.end(), drawElement),
		m_drawElements.end()
	);
}

void DrawElementManager::DrawElements(ImDrawList* drawList)
{
	for (ADrawElement* drawElement : m_drawElements)
	{
		drawElement->Draw(drawList);
	}
}
