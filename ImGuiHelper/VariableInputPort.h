#pragma once
#include "VariablePort.h"
#include <type_traits>

template <typename OutputType>
class VariableOutputPort;

template<typename InputType>
class VariableInputPort : public VariablePort<InputType>
{
	using VP = VariablePort<InputType>;

public:
	VariableInputPort(
		Node* parentNode, size_t indexCount, size_t portIndex,
		const float& radius, const ImVec2& referencedOrigin
	);
	~VariableInputPort() override = default;

protected:
	VariableOutputPort<InputType>* m_connectedPort = nullptr;

public:
	inline void SetConnectedOutputPort(VariableOutputPort<InputType>* outputPort);

protected:
	virtual void DrawPortConnection(ImDrawList* drawListIn) override;

public:
	virtual void OnBeginDrag() override;
	virtual void OnEndDrag() override;
};


template<typename InputType>
inline VariableInputPort<InputType>::VariableInputPort(
	Node* parentNode, size_t indexCount, 
	size_t portIndex, const float& radius, const ImVec2& referencedOrigin
)
	: VP(parentNode, true, indexCount, portIndex, radius, referencedOrigin)
{
}

template<typename InputType>
inline void VariableInputPort<InputType>::SetConnectedOutputPort(VariableOutputPort<InputType>* outputPort)
{
	m_connectedPort = outputPort; 
}

template<typename InputType>
inline void VariableInputPort<InputType>::DrawPortConnection(ImDrawList* drawListIn)
{
	if (m_connectedPort != nullptr || VP::m_isConnecting)
	{
		if (m_connectedPort != nullptr)
		{
			VP::m_mousePositionDuringConnect = m_connectedPort->GetDrawCenter();
		}

		VP::DrawBezierForConnection(drawListIn, VP::m_drawCenter, VP::m_mousePositionDuringConnect);
	}
}

template<typename InputType>
inline void VariableInputPort<InputType>::OnBeginDrag()
{
	VP::OnBeginDrag();
	VP::m_connectingInputPort = this;
	m_connectedPort = nullptr;
}

template<typename InputType>
inline void VariableInputPort<InputType>::OnEndDrag()
{
	VP::OnEndDrag();

	if (VP::m_connectingOutputPort != nullptr)
	{
		if (VP::m_connectingOutputPort->IsConnectable(this))
		{
			SetConnectedOutputPort((VariableOutputPort<InputType>*)VP::m_connectingOutputPort);
		}
	}
	VP::m_connectingOutputPort = nullptr;
}
