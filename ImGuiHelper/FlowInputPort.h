#pragma once
#include "FlowPort.h"

class FlowOutputPort;

class FlowInputPort : public FlowPort
{
public:
	template<typename ...InputTypes>
	FlowInputPort(
		FlowNode<InputTypes...>* parentNode, size_t indexCount, size_t portIndex,
		const float& radius, const ImVec2& referencedOrigin
	);
	~FlowInputPort() override = default;

protected:
	FlowOutputPort* m_connectedPort = nullptr;
	ImVec2 m_typeTextSize;

public:
	inline void SetConnectedOutputPort(FlowOutputPort* outputPort) { m_connectedPort = outputPort; }
	inline FlowOutputPort* GetConnectedOutputPort() { return m_connectedPort; }

protected:
	virtual void DrawPortConnection(ImDrawList* drawListIn) override;

public:
	virtual void OnMouseUp(MouseClickEventArgs& args) override;
	virtual void OnBeginDrag() override;
	virtual void OnEndDrag() override;

};

template<typename ...InputTypes>
inline FlowInputPort::FlowInputPort(
	FlowNode<InputTypes...>* parentNode, size_t indexCount, size_t portIndex, 
	const float& radius, const ImVec2& referencedOrigin
)
	: FlowPort(parentNode, true, indexCount, portIndex, radius, referencedOrigin)
{
}