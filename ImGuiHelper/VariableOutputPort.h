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
	// TODO : TO BE Interface
	virtual OutputType GetVariable() const { return OutputType(); }

public:
	virtual void OnBeginDrag() override;
	virtual void OnEndDrag() override;
};

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
inline void VariableOutputPort<OutputType>::OnBeginDrag()
{
	VP::OnBeginDrag();
	VP::m_connectingOutputPort = this;
}

template<typename OutputType>
inline void VariableOutputPort<OutputType>::OnEndDrag()
{
	VP::m_connectingOutputPort = this;
	VP::m_connectingInputPort->OnEndDrag();
	VP::m_connectingInputPort = nullptr;
}
