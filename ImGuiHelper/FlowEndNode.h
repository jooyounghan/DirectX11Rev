#pragma once
#include "FlowNode.h"

class FlowEndNode : public FlowNode<>
{
public:
	FlowEndNode(const ImVec2& leftTop, const float& radius, const ImVec2& referencedOrigin);
	~FlowEndNode() override = default;

public:
	virtual void AddToDrawElementManager(DrawElementManager* drawElementManager) override;
	virtual void RemoveFromDrawElementManager(DrawElementManager* drawElementManager) override;
	virtual void RegisterToInteractionManager(InteractionManager* interactionManager) override;
	virtual void DeregisterToInteractionManager(InteractionManager* interactionManager) override;

protected:
	virtual void SetFocused(const bool& isFocused) override;
	virtual ImVec2 GetInternalNodeSize() override;

protected:
	virtual void ExecuteImpl() override {};
};
