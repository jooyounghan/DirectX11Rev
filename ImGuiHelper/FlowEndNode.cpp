#include "FlowEndNode.h"
#include "FlowOutputPort.h"

FlowEndNode::FlowEndNode(const ImVec2& leftTop, const float& radius, const ImVec2& referencedOrigin)
	: FlowNode("End", leftTop, radius, referencedOrigin)
{
}

void FlowEndNode::AddToDrawElementManager(DrawElementManager* drawElementManager)
{
	Node::AddToDrawElementManager(drawElementManager);
	m_flowInputPort.AddToDrawElementManager(drawElementManager);
}

void FlowEndNode::RemoveFromDrawElementManager(DrawElementManager* drawElementManager)
{
	Node::RemoveFromDrawElementManager(drawElementManager);
	m_flowInputPort.RemoveFromDrawElementManager(drawElementManager);
}

void FlowEndNode::RegisterToInteractionManager(InteractionManager* interactionManager)
{
	Node::RegisterToInteractionManager(interactionManager);
	m_flowInputPort.RegisterToInteractionManager(interactionManager);
}

void FlowEndNode::DeregisterToInteractionManager(InteractionManager* interactionManager)
{
	Node::DeregisterToInteractionManager(interactionManager);
	m_flowInputPort.DeregisterToInteractionManager(interactionManager);
}

void FlowEndNode::SetFocused(const bool& isFocused)
{
	Node::SetFocused(isFocused);
	m_flowInputPort.SetFocused(isFocused);
}

ImVec2 FlowEndNode::GetInternalNodeSize()
{
	return ImVec2(nodeMinWidth, nodeMinHeight);
}
