#pragma once
#include "ADrawElement.h"

#include <vector>

class DrawElementManager
{
public:
	virtual ~DrawElementManager();

	friend class ADrawElement;

private:
	std::vector<ADrawElement*> m_drawElements;

protected:
	virtual void AddDrawElement(ADrawElement* drawElement);
	void RemoveDrawElement(ADrawElement* drawElement);
	void BringDrawElementToFront(ADrawElement* drawElement);

protected:
	void DrawElements(ImDrawList* drawList);
};

