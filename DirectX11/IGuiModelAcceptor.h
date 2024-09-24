#pragma once
class IGuiModelVisitor;

class IGuiModelAcceptor
{
public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) = 0;
};