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
		Node* parentNode, size_t indexCount, size_t portIndex, const float& radius, 
		const ImVec2& referencedOrigin, const std::string& description
	);
	~VariableInputPort() override = default;

protected:
	VariableOutputPort<InputType>* m_connectedPort = nullptr;
	std::string m_description;
	ImVec2 m_descriptionSize;

public:
	inline void SetConnectedOutputPort(VariableOutputPort<InputType>* outputPort) { m_connectedPort = outputPort; };
	inline VariableOutputPort<InputType>* GetConnectedOutputPort() const { return m_connectedPort; }
	inline const ImVec2& GetTypeTextSize() { return m_descriptionSize; }

protected:
	virtual void DrawPortConnection(ImDrawList* drawListIn) override;

public:
	virtual void Draw(ImDrawList* drawListIn) override;
	virtual void OnMouseUp(MouseClickEventArgs& args) override;
	virtual void OnBeginDrag() override;
	virtual void OnEndDrag() override;
};

template<typename InputType>
inline VariableInputPort<InputType>::VariableInputPort(
	Node* parentNode, size_t indexCount, size_t portIndex, const float& radius, 
	const ImVec2& referencedOrigin, const std::string& description
)
	: VP(parentNode, true, indexCount, portIndex, radius, referencedOrigin)
{
	m_description = description;
	m_descriptionSize = ImGui::CalcTextSize(m_description.c_str());
}

template<typename InputType>
inline void VariableInputPort<InputType>::Draw(ImDrawList* drawListIn)
{
	VP::Draw(drawListIn);
	ImVec2 textDrawPosition = ImVec2(VP::m_drawCenter.x + VP::m_radius, VP::m_drawCenter.y - m_descriptionSize.y / 2.f);
	drawListIn->AddText(textDrawPosition, textColor, m_description.c_str());
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
inline void VariableInputPort<InputType>::OnMouseUp(MouseClickEventArgs& args)
{
	VP::OnMouseUp(args);

	if (VP::m_connectingOutputPort != nullptr)
	{
		if (VP::m_connectingOutputPort->IsConnectable(this))
		{
			SetConnectedOutputPort((VariableOutputPort<InputType>*)VP::m_connectingOutputPort);
		}
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
	VP::m_connectingInputPort = nullptr;

}

