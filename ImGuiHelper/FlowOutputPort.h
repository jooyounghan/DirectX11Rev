#pragma once
#include "FlowPort.h"

class FlowOutputPort : public FlowPort
{
public:
	template<typename ...InputTypes>
	FlowOutputPort(
		FlowNode<InputTypes...>* parentNode, size_t indexCount, size_t portIndex,
		const float& radius, const ImVec2& referencedOrigin
	);
	~FlowOutputPort() override = default;

protected:
	virtual void DrawPortConnection(ImDrawList* drawListIn) override;

public:
	virtual void OnMouseUp(MouseClickEventArgs& args) override;

public:
	virtual void OnBeginDrag() override;
	virtual void OnEndDrag() override;
};

template<typename ...InputTypes>
inline FlowOutputPort::FlowOutputPort(
	FlowNode<InputTypes...>* parentNode, size_t indexCount, size_t portIndex,
	const float& radius, const ImVec2& referencedOrigin
)
	: FlowPort(parentNode, false, indexCount, portIndex, radius, referencedOrigin)
{
}