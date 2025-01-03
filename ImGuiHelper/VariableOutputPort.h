#pragma once
#include "VariablePort.h"
#include <type_traits>

template <typename OutputType>
class VariableInputPort;

template<typename OutputType>
class VariableOutputPort : public VariablePort<OutputType>
{
	using VP = VariablePort<OutputType>;

public:
	VariableOutputPort(
		Node* parentNode, size_t indexCount, size_t portIndex,
		const float& radius, const ImVec2& referencedOrigin
	);
	~VariableOutputPort() override = default;

protected:
	virtual void DrawPortConnection(ImDrawList* drawListIn) override;

public:
	virtual OutputType GetVariable() const = 0;

public:
	virtual void OnMouseUp(MouseClickEventArgs& args) override;

public:
	virtual void OnBeginDrag() override;
	virtual void OnEndDrag() override;
};

template<typename OutputType, typename Derived>
concept VariableOutPortType = std::is_base_of_v<VariableOutputPort<OutputType>, Derived>;


template<typename OutputType>
inline VariableOutputPort<OutputType>::VariableOutputPort(
	Node* parentNode, size_t indexCount,
	size_t portIndex, const float& radius, const ImVec2& referencedOrigin
)
	: VP(parentNode, false, indexCount, portIndex, radius, referencedOrigin)
{
}

template<typename OutputType>
inline void VariableOutputPort<OutputType>::DrawPortConnection(ImDrawList* drawListIn)
{
	if (VP::m_isConnecting)
	{
		VP::DrawBezierForConnection(drawListIn, VP::m_drawCenter, VP::m_mousePositionDuringConnect);
	}
}

template<typename OutputType>
inline void VariableOutputPort<OutputType>::OnMouseUp(MouseClickEventArgs& args)
{
	VP::OnMouseUp(args);

	if (VP::m_connectingInputPort != nullptr)
	{
		VP::m_connectingOutputPort = this;
		VP::m_connectingInputPort->OnMouseUp(args);
		VP::m_connectingOutputPort = nullptr;
	}
}

template<typename OutputType>
inline void VariableOutputPort<OutputType>::OnBeginDrag()
{
	VP::OnBeginDrag();
	VP::m_connectingOutputPort = this;
}

template<typename OutputType>
inline void VariableOutputPort<OutputType>::OnEndDrag()
{
	VP::OnEndDrag();
	VP::m_connectingOutputPort = nullptr;
}
