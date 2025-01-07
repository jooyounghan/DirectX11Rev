#include "FlowEndNode.h"
#include "FlowOutputPort.h"
#include "NodeConstant.h"

FlowEndNode::FlowEndNode(const ImVec2& leftTop, const float& radius, const ImVec2& referencedOrigin)
	: FlowNode("End", leftTop, radius, referencedOrigin, {})
{
	AddDrawCommand([&](const ImVec2& drawLeftTop, ImDrawList* drawListIn)
		{			
			return ImVec2(defaultNodeWidth, defaultNodeHeight);
		}
	);
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
